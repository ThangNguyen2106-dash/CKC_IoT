#ifndef CKC_ESP32
#define CKC_ESP32

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#define WIFI_MAX 5
#define FLASH_BTN 0

enum CKC_WiFI_TASK
{
    MODE_STA,
    MODE_AP,
    MODE_CONNECTED
};

CKC_WiFI_TASK WiFi_TASK = MODE_STA;

template <class Transport>
class CKC_PnP
{
public:
    void init();
    void run();

private:
    WebServer server{80};
    Preferences prefs;

    String ssid_list[WIFI_MAX];
    String pass_list[WIFI_MAX];

    char ap_ssid[32];
    const char *ap_pass = "12345678";

    unsigned long t0;

    void loadWiFi();
    void saveWiFi(String ssid, String pass);
    bool connectMultiWiFi();

    void startAP();
    void handleAP();
    void setupWeb();

    void handleButton();
};

// ===== INIT =====
template <class T>
void CKC_PnP<T>::init()
{
    Serial.begin(115200);

    loadWiFi();

    String mac = WiFi.macAddress();
    mac.replace(":", "");
    snprintf(ap_ssid, sizeof(ap_ssid), "CKC_%s", mac.c_str());

    pinMode(FLASH_BTN, INPUT_PULLUP);
}

// ===== LOAD WIFI =====
template <class T>
void CKC_PnP<T>::loadWiFi()
{
    prefs.begin("wifi", true);
    for (int i = 0; i < WIFI_MAX; i++)
    {
        ssid_list[i] = prefs.getString(("ssid" + String(i)).c_str(), "");
        pass_list[i] = prefs.getString(("pass" + String(i)).c_str(), "");
    }
    prefs.end();
}

// ===== SAVE WIFI =====
template <class T>
void CKC_PnP<T>::saveWiFi(String ssid, String pass)
{
    prefs.begin("wifi", false);

    for (int i = WIFI_MAX - 1; i > 0; i--)
    {
        prefs.putString(("ssid" + String(i)).c_str(), ssid_list[i - 1]);
        prefs.putString(("pass" + String(i)).c_str(), pass_list[i - 1]);
    }

    prefs.putString("ssid0", ssid);
    prefs.putString("pass0", pass);

    prefs.end();
}

// ===== MULTI WIFI CONNECT =====
template <class T>
bool CKC_PnP<T>::connectMultiWiFi()
{
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

// ===== START AP =====
template <class T>
void CKC_PnP<T>::startAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_pass);

    Serial.println("AP MODE");
    Serial.println(ap_ssid);

    setupWeb();
}

// ===== WEB SERVER =====
template <class T>
void CKC_PnP<T>::setupWeb()
{
    server.on("/", [this]()
              {
        String html = "<h2>WiFi Config</h2>";
        html += "<form action='/save'>";
        html += "SSID:<input name='ssid'><br>";
        html += "PASS:<input name='pass'><br>";
        html += "<input type='submit'></form>";
        server.send(200, "text/html", html); });

    server.on("/save", [this]()
              {
        String s = server.arg("ssid");
        String p = server.arg("pass");

        saveWiFi(s, p);

        server.send(200, "text/html", "Saved! Restart...");
        delay(2000);
        WiFi_TASK = MODE_STA; });

    server.begin();
}

// ===== HANDLE AP =====
template <class T>
void CKC_PnP<T>::handleAP()
{
    server.handleClient();
}

// ===== BUTTON =====
template <class T>
void CKC_PnP<T>::handleButton()
{
    static unsigned long pressTime = 0;

    if (digitalRead(FLASH_BTN) == LOW)
    {
        if (pressTime == 0)
            pressTime = millis();

        if (millis() - pressTime > 5000)
        {
            Serial.println("Force AP");
            WiFi_TASK = MODE_AP;
            pressTime = 0;
        }
    }
    else
    {
        pressTime = 0;
    }
}

// ===== RUN =====
template <class T>
void CKC_PnP<T>::run()
{
    switch (WiFi_TASK)
    {
    case MODE_STA:
        if (connectMultiWiFi())
        {
            WiFi_TASK = MODE_CONNECTED;
        }
        else
        {
            WiFi_TASK = MODE_AP;
            startAP();
        }
        break;

    case MODE_AP:
        handleAP();
        break;

    case MODE_CONNECTED:
        if (WiFi.status() != WL_CONNECTED)
        {
            WiFi_TASK = MODE_STA;
        }
        break;
    }

    handleButton();
}

#endif