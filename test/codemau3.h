#ifndef CKC_ESP32
#define CKC_ESP32

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "IoT_Web.h"
#include <Preferences.h>
#include <WiFiClient.h>
#include <CKC/CKC_API.hpp>
#include <MQTT/ESP32_MQTT.hpp>

//================ DEFINE =================//
#define WIFI_AP_IP IPAddress(192, 168, 27, 1)
#define WIFI_AP_Subnet IPAddress(255, 255, 255, 0)

#define STA_WIFI_PORT "80"

#define AP_WIFI_NAME "CKC:"
#define AP_WIFI_PASS "CKC@2026"
#define AP_WIFI_IP "192.168.1.4"
#define AP_WIFI_PORT "80"

#define FLASH_BTN 0
#define WIFI_MAX 5

//================ STATE =================//
enum CKC_WiFI_TASK
{
    MODE_CONFIG_WIFI,
    MODE_STA,
    MODE_AP,
    MODE_AP_STA,
    MODE_CONNECTED,
    WIFI_DISCONNECTED,
};

CKC_WiFI_TASK WiFi_TASK = MODE_CONFIG_WIFI;

//================ CLASS =================//
template <class Transport>
class CKC_PnP
{
public:
    void init(const char *sta_ssid, const char *sta_pass);
    void run();

    void handleSave();
    void SaveWiFi(String newSSID, String newPASS);
    void loadWiFi();
    bool connectMultiWiFi();
    void setup_APWeb_WiFi();

private:
    // STA
    char _sta_ssid[64];
    char _sta_pass[32];
    char _sta_port[5] = STA_WIFI_PORT;

    // AP
    char _ap_ssid[64] = AP_WIFI_NAME;
    char _ap_pass[32] = AP_WIFI_PASS;
    char _ap_ip[16] = AP_WIFI_IP;
    char _ap_port[5] = AP_WIFI_PORT;

    char _mac[20];
    IPAddress _ipAddr;

    // time
    unsigned long t0, t1, t2;

    // button
    unsigned long pressStart = 0;
    bool triggered = false;

    // web
    WebServer *server = nullptr;

    // storage
    Preferences prefs;
    String ssid_list[WIFI_MAX];
    String pass_list[WIFI_MAX];

    // state flag
    bool ap_started = false;
    bool sta_ok = false;

    // func
    void CKC_CONNECTION();
    void CKC_state_Connect_STA();
    void CKC_state_Connect_AP();
    void CKC_mode_WiFi_connected();
    void CKC_mode_Config();
    void handler_button();
};

//================ INIT =================//
template <class Transport>
void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass)
{
    server = new WebServer(80);

    strcpy(_sta_ssid, sta_ssid);
    strcpy(_sta_pass, sta_pass);

    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());

    snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", AP_WIFI_NAME, _mac);

    pinMode(FLASH_BTN, INPUT_PULLUP);

    WiFi_TASK = MODE_CONFIG_WIFI;
}

//================ WIFI STORAGE =================//
template <class Transport>
void CKC_PnP<Transport>::SaveWiFi(String newSSID, String newPASS)
{
    prefs.begin("wifi", false);

    for (int i = WIFI_MAX - 1; i > 0; i--)
    {
        prefs.putString(("ssid" + String(i)).c_str(), ssid_list[i - 1]);
        prefs.putString(("pass" + String(i)).c_str(), pass_list[i - 1]);
    }

    prefs.putString("ssid0", newSSID);
    prefs.putString("pass0", newPASS);

    prefs.end();
}

template <class Transport>
void CKC_PnP<Transport>::loadWiFi()
{
    prefs.begin("wifi", true);

    for (int i = 0; i < WIFI_MAX; i++)
    {
        ssid_list[i] = prefs.getString(("ssid" + String(i)).c_str(), "");
        pass_list[i] = prefs.getString(("pass" + String(i)).c_str(), "");
    }

    prefs.end();
}

//================ MULTI WIFI =================//
template <class Transport>
bool CKC_PnP<Transport>::connectMultiWiFi()
{
    loadWiFi();

    WiFi.mode(WIFI_STA);

    for (int i = 0; i < WIFI_MAX; i++)
    {
        if (ssid_list[i] == "")
            continue;

        Serial.println("Try: " + ssid_list[i]);

        WiFi.begin(ssid_list[i].c_str(), pass_list[i].c_str());

        unsigned long start = millis();

        while (millis() - start < 8000)
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.println("Connected: " + ssid_list[i]);
                return true;
            }
            delay(200);
        }
    }
    return false;
}

//================ WEB =================//
template <class Transport>
void CKC_PnP<Transport>::handleSave()
{
    String newSSID = server->arg("ssid");
    String newPASS = server->arg("pass");

    SaveWiFi(newSSID, newPASS);

    server->send(200, "text/html", "Saved!");
    delay(2000);
    ESP.restart();
}

template <class Transport>
void CKC_PnP<Transport>::setup_APWeb_WiFi()
{
    server->on("/", [this]()
               { server->send(200, "text/html", htmlPage()); });

    server->on("/save", [this]()
               { this->handleSave(); });

    server->begin();
}

//================ STATE =================//
template <class Transport>
void CKC_PnP<Transport>::CKC_CONNECTION()
{
    WiFi_TASK = MODE_STA;
}

template <class Transport>
void CKC_PnP<Transport>::CKC_state_Connect_STA()
{
    if (connectMultiWiFi())
    {
        WiFi_TASK = MODE_CONNECTED;
    }
    else
    {
        WiFi_TASK = MODE_AP;
    }
}

template <class Transport>
void CKC_PnP<Transport>::CKC_state_Connect_AP()
{
    if (!ap_started)
    {
        ap_started = true;

        WiFi.mode(WIFI_AP);
        WiFi.softAP(_ap_ssid, _ap_pass);

        setup_APWeb_WiFi();

        CKC_LOG_DEBUG("WIFI", "AP MODE");
        CKC_LOG_DEBUG("WIFI", "SSID: %s", _ap_ssid);
    }

    server->handleClient();
}

template <class Transport>
void CKC_PnP<Transport>::CKC_mode_WiFi_connected()
{
    static bool once = false;

    if (!once)
    {
        once = true;
        serverMQTT.begin();

        CKC_LOG_DEBUG("WIFI", "CONNECTED");
        CKC_LOG_DEBUG("WIFI", "IP: %s", WiFi.localIP().toString().c_str());
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        once = false;
        WiFi_TASK = MODE_CONFIG_WIFI;
    }

    serverMQTT.run();
}

template <class Transport>
void CKC_PnP<Transport>::CKC_mode_Config()
{
    // dùng khi cần mở rộng
}

//================ BUTTON =================//
template <class Transport>
void CKC_PnP<Transport>::handler_button()
{
#ifdef BUTTON_MODE
    bool pressed = (digitalRead(FLASH_BTN) == LOW);

    if (pressed)
    {
        if (pressStart == 0)
            pressStart = millis();

        if (!triggered && millis() - pressStart > 5000)
        {
            triggered = true;
            WiFi_TASK = MODE_AP;
            ap_started = false;
        }
    }
    else
    {
        pressStart = 0;
        triggered = false;
    }
#endif
}

//================ RUN =================//
template <class Transport>
void CKC_PnP<Transport>::run()
{
    switch (WiFi_TASK)
    {
    case MODE_CONFIG_WIFI:
        CKC_CONNECTION();
        break;

    case MODE_STA:
        CKC_state_Connect_STA();
        break;

    case MODE_AP:
        CKC_state_Connect_AP();
        break;

    case MODE_CONNECTED:
        CKC_mode_WiFi_connected();
        break;

    default:
        break;
    }

    handler_button();
}

#endif