#ifndef CKC_ESP32
#define CKC_ESP32
#include <Arduino.h>
// #include <IPAddress.h>
// #include <WiFiAP.h>
#include <WiFiClient.h>
#include <CKC/CKC_API.hpp>
#include <MQTT/ESP32_MQTT.hpp>

#include "IoT_Web.h"
#include <WebServer.h>
#include <Preferences.h>

#define WIFI_AP_IP IPAddress(192, 168, 27, 1)
#define WIFI_AP_Subnet IPAddress(255, 255, 255, 0)

char STA_WIFI_NAME[32];
char STA_WIFI_PASS[32];
#define STA_WIFI_PORT "80"

#define AP_WIFI_NAME "CKC:"
#define AP_WIFI_PASS "CKC@2026"
#define AP_WIFI_IP "192.168.6.1"
#define AP_WIFI_PORT "80"

enum CKC_WiFI_TASK
{
    MODE_CONFIG_WIFI,
    MODE_STA,
    MODE_AP,
    MODE_CONNECTED,
    RUN_AP_WEB,
    WIFI_DISCONNECTED,
};

CKC_WiFI_TASK WiFi_TASK = MODE_STA;
template <class Transport>
class CKC_PnP
{
    WebServer webServer{80};
    Preferences prefs;

public:
    CKC_PnP() {};
    void init(const char *sta_ssid, const char *sta_pass);
    void SaveWiFi(String newSSID, String newPASS);
    void handleSave();
    void loadWiFi();

    void CKC_state_Connect_STA();
    void CKC_state_Connect_AP();

    void CKC_mode_connected();
    bool CkC_Connected();
    void run();

    void handler_button();

private:
    IPAddress _ipAddr;

    // sta
    char _sta_ssid[64];
    char _sta_pass[32];
    char _sta_ip[16];
    char _sta_port[5] = STA_WIFI_PORT;
    int _rssi;
    // ap
    char _ap_ssid[64] = AP_WIFI_NAME;
    char _ap_pass[32] = AP_WIFI_PASS;
    char _ap_ip[16] = AP_WIFI_IP;
    char _ap_port[5] = AP_WIFI_PORT;
    char _mac[18];
    // var local
    // unsigned long _SendRssiTime;
    // unsigned int count_wifiConnect;
    unsigned long t0, t1, t2, t3, t4;
    // String _ping;
    int time_sta = 20000;
#define FLASH_BTN 0 // nút BOOT/FLASH trên ESP32 thường là GPIO0

    unsigned long pressStart = 0;
    bool triggered = false;

    String newSSID;
    String newPASS;
#define WiFi_MAX 5
    String ssid_list[WiFi_MAX];
    String pass_list[WiFi_MAX];
    // bool apMode = false;
};
//========== STA MODE ==========//
template <class Transport>
inline void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass)
{
    strcpy(_sta_ssid, sta_ssid);
    strcpy(_sta_pass, sta_pass);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", _ap_ssid, _mac);

    CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", _sta_ip);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
    t1 = millis();
    pinMode(FLASH_BTN, INPUT_PULLUP); // nút kéo xuống GND khi nhấn
}

template <class Transport>
inline void CKC_PnP<Transport>::SaveWiFi(String newSSID, String newPASS)
{
    prefs.begin("wifi", false);
    for (int i = WiFi_MAX - 1; i > 0; i--)
    {
        prefs.putString(("ssid" + String(i)).c_str(), ssid_list[i - 1]);
        prefs.putString(("pass" + String(i)).c_str(), pass_list[i - 1]);
    }
    prefs.putString("ssid0", newSSID);
    prefs.putString("pass0", newPASS);
    prefs.end();
}

template <class Transport>
inline void CKC_PnP<Transport>::handleSave()
{
    String newSSID = webServer.arg("ssid");
    String newPASS = webServer.arg("pass");
    SaveWiFi(newSSID, newPASS);

    strcpy(_sta_ssid, newSSID.c_str());
    strcpy(_sta_pass, newPASS.c_str());

    webServer.send(200, "text/html", "Saved! Restarting...");
    delay(1000);
    WiFi_TASK = MODE_STA;
}

template <class Transport>
inline void CKC_PnP<Transport>::loadWiFi()
{
    prefs.begin("wifi", true);
    for (int i = 0; i < WiFi_MAX; i++)
    {
        ssid_list[i] = prefs.getString(("ssid" + String(i)).c_str(), "");
        pass_list[i] = prefs.getString(("pass" + String(i)).c_str(), "");
    }
    prefs.end();
}

template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_STA()
{
    webServer.stop();
    loadWiFi();
    // ==============================
    //     ƯU TIÊN WIFI TỪ INIT()
    // ==============================
    if (strlen(_sta_ssid) > 0)
    {
        t1 = millis();
        WiFi.mode(WIFI_STA);
        WiFi.disconnect(true);
        delay(200);
        WiFi.begin(_sta_ssid, _sta_pass);

        CKC_LOG_DEBUG("WIFI", "TRY CONNECTING TO: %s", _sta_ssid);

        while (WiFi.status() != WL_CONNECTED && millis() - t1 <= this->time_sta)
        {
            if (millis() - t0 > 1000)
            {
                CKC_LOG_DEBUG("WIFI", "CONNECTING ___ %ds\r", (millis() - t1) / 1000);
                t0 = millis();
            }
            if (WiFi_TASK == MODE_AP)
            {
                CKC_LOG_DEBUG("WIFI", "BREAK WIFI CONNECT !!!!!! ");
                return;
            }
            delay(10);
            handler_button();
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            SaveWiFi(String(_sta_ssid), String(_sta_pass));
            CKC_LOG_DEBUG("WIFI", "WIFI_CONNECTED :)) ");
            CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", WiFi.localIP().toString());
            CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
            serverMQTT.begin();
            CKC_LOG_DEBUG("TAG", "\r\n"
                                 "  ____  _  __   ____   "
                                 "\r\n"
                                 " / ___|| |/ /  / ___|  "
                                 "\r\n"
                                 "| |    | ' /  | |      "
                                 "\r\n"
                                 "| |___ | . \\  | |___   "
                                 "\r\n"
                                 " \\____||_|\\_\\  \\____|  "
                                 "\r\n");
            WiFi_TASK = MODE_CONNECTED;
            return;
        }
    }
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);
    delay(200);
    Serial.println("Scanning WiFi...");
    int n = WiFi.scanNetworks();
    if (n <= 0)
    {
        CKC_LOG_DEBUG("WIFI", "NO_WIFI_CONNECT !!!!!! ");
        mqttClient.disconnect();
        WiFi_TASK = MODE_AP;
        return;
    }
    for (int j = 0; j < WiFi_MAX; j++)
    {
        if (ssid_list[j] == "")
            continue;
        for (int i = 0; i < n; i++)
        {
            if (WiFi.SSID(i) == ssid_list[j])
            {
                CKC_LOG_DEBUG("WIFI", "Connecting to: %s", ssid_list[j].c_str());
                t1 = millis();
                WiFi.begin(ssid_list[j].c_str(), pass_list[j].c_str());
                while (WiFi.status() != WL_CONNECTED && millis() - t1 <= this->time_sta)
                {
                    if (millis() - t0 > 1000)
                    {
                        CKC_LOG_DEBUG("WIFI", "CONNECTING ___ %ds\r", (millis() - t1) / 1000);
                        t0 = millis();
                    }
                    if (WiFi_TASK == MODE_AP)
                    {
                        CKC_LOG_DEBUG("WIFI", "BREAK WIFI CONNECT !!!!!! ");
                        return;
                    }
                    delay(10);
                    handler_button();
                }
                if (WiFi.status() == WL_CONNECTED)
                {
                    CKC_LOG_DEBUG("WIFI", "WIFI_CONNECTED :)) ");
                    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", WiFi.localIP().toString());
                    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
                    serverMQTT.begin();
                    CKC_LOG_DEBUG("TAG", "\r\n"
                                         "  ____  _  __   ____   "
                                         "\r\n"
                                         " / ___|| |/ /  / ___|  "
                                         "\r\n"
                                         "| |    | ' /  | |      "
                                         "\r\n"
                                         "| |___ | . \\  | |___   "
                                         "\r\n"
                                         " \\____||_|\\_\\  \\____|  "
                                         "\r\n");
                    WiFi_TASK = MODE_CONNECTED;
                    return;
                }
                else
                {
                    CKC_LOG_DEBUG("WIFI", "WIFI_CONNECT_FALSE !!!!!! ");
                    CKC_LOG_DEBUG("WIFI", "TRY_ANOTHER_WIFI!!!");
                }
            }
        }
    }
    CKC_LOG_DEBUG("WIFI", "NO_WIFI_CONNECT !!!!!! ");
    mqttClient.disconnect();
    WiFi_TASK = MODE_AP;
    delay(100);
}
//========== AP MODE ==========//
template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_AP()
{
    CKC_LOG_DEBUG("WIFI", "RUN_AP");
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(100);
    WiFi.mode(WIFI_AP);
    IPAddress local_ip;
    local_ip.fromString(_ap_ip);
    WiFi.softAPConfig(local_ip, local_ip, WIFI_AP_Subnet);
    WiFi.softAP(_ap_ssid, _ap_pass);
    webServer.on("/", [this]()
                 { this->webServer.send(200, "text/html", htmlPage()); });

    webServer.on("/save", [this]()
                 { this->handleSave(); });
    webServer.begin();
    mqttClient.disconnect();
    delay(1000);
    WiFi_TASK = RUN_AP_WEB;
}
//========== CONNECTED MODE ==========//

template <class Transport>
inline bool CKC_PnP<Transport>::CkC_Connected()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
    else
    {
        return false;
    }
}
template <class Transport>
inline void CKC_PnP<Transport>::CKC_mode_connected()
{
    if (!CkC_Connected())
    {
        mqttClient.disconnect();
        WiFi_TASK = MODE_STA;
        return;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        serverMQTT.run();
        if (!serverMQTT._connect())
        {
            unsigned long lastMQTTReconnect = 0;
            if (millis() - lastMQTTReconnect > 5000)
            {
                lastMQTTReconnect = millis();
                serverMQTT.begin();
            }
        }
    }
}

//================ BUTTON ================
template <class Transport>
inline void CKC_PnP<Transport>::handler_button()
{
#ifdef BUTTON_MODE
    bool pressed = (digitalRead(FLASH_BTN) == LOW);
    if (pressed)
    {
        if (pressStart == 0)
            pressStart = millis();

        if (!triggered && (millis() - pressStart >= 5000))
        {
            triggered = true;
            mqttClient.disconnect();
            WiFi_TASK = MODE_AP;
            CKC_LOG_DEBUG("WIFI", "BUTTON TRIGGER");
        }
    }
    else
    {
        if (triggered)
        {
            pressStart = 0;
            triggered = false;
        }
    }

#endif
}

//================ RUN ================
template <class Transport>
inline void CKC_PnP<Transport>::run()
{
    switch (WiFi_TASK)
    {
    case MODE_STA:
        this->CKC_state_Connect_STA();
        break;
    case MODE_AP:
        this->CKC_state_Connect_AP();
        break;
    case MODE_CONNECTED:
        this->CKC_mode_connected();
        break;
    case RUN_AP_WEB:
        mqttClient.disconnect();
        this->webServer.handleClient();
        break;
    default:
        break;
    }
    this->handler_button();
}

#endif