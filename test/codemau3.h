#ifndef CKC_ESP32
#define CKC_ESP32

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

//================ CONFIG ================//
#define WIFI_MAX 5
#define FLASH_BTN 0

#define WIFI_AP_IP IPAddress(192, 168, 27, 1)
#define WIFI_AP_Subnet IPAddress(255, 255, 255, 0)

#define AP_WIFI_NAME "CKC:"
#define AP_WIFI_PASS "CKC@2026"

//================ TASK ENUM ================//
enum CKC_WiFI_TASK
{
    MODE_STA,
    MODE_AP,
    MODE_CONNECTED,
    MODE_AP_STA
};

CKC_WiFI_TASK WiFi_TASK = MODE_STA;

//================ CLASS ================//
template <class Transport>
class CKC_PnP
{
public:
    void init(const char *sta_ssid, const char *sta_pass);
    void run();

private:
    //====== WiFi ======//
    char _sta_ssid[32];
    char _sta_pass[32];

    char _ap_ssid[64] = AP_WIFI_NAME;
    char _ap_pass[32] = AP_WIFI_PASS;

    WebServer *server = nullptr;
    Preferences prefs;

    String ssid_list[WIFI_MAX];
    String pass_list[WIFI_MAX];

    //====== TIME ======//
    unsigned long t0;
    const int time_sta = 20000;

    //====== BUTTON ======//
    unsigned long pressStart = 0;
    bool triggered = false;

    //====== STATE ======//
    void TASK_STA();
    void TASK_AP();
    void TASK_CONNECTED();
    void TASK_AP_STA();

    //====== FUNC ======//
    void loadWiFi();
    bool connectMultiWiFi();

    void setupAP();
    void setupWeb();

    void handleSave();
    void saveWiFi(String ssid, String pass);

    void handler_button();
};

//================ INIT ================//
template <class Transport>
void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass)
{
    server = new WebServer(80);

    strcpy(_sta_ssid, sta_ssid);
    strcpy(_sta_pass, sta_pass);

    String mac = WiFi.macAddress();
    strcat(_ap_ssid, mac.c_str());

    pinMode(FLASH_BTN, INPUT_PULLUP);

    Serial.println("INIT DONE");
}

//================ LOAD WIFI ================//
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

//================ SAVE WIFI ================//
template <class Transport>
void CKC_PnP<Transport>::saveWiFi(String ssid, String pass)
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

//================ CONNECT MULTI WIFI ================//
template <class Transport>
bool CKC_PnP<Transport>::connectMultiWiFi()
{
    loadWiFi();

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
                Serial.println("Connected!");
                return true;
            }
            delay(200);
        }
    }
    return false;
}

//================ WEB SETUP ================//
template <class Transport>
void CKC_PnP<Transport>::setupWeb()
{
    server->on("/", [this]()
               { server->send(200, "text/html", "<form action='/save'>SSID:<input name='ssid'><br>PASS:<input name='pass'><br><input type='submit'></form>"); });

    server->on("/save", [this]()
               { handleSave(); });

    server->begin();
}

//================ HANDLE SAVE ================//
template <class Transport>
void CKC_PnP<Transport>::handleSave()
{
    String ssid = server->arg("ssid");
    String pass = server->arg("pass");

    saveWiFi(ssid, pass);

    server->send(200, "text/html", "Saved! Rebooting...");
    delay(1000);
    ESP.restart();
}

//================ AP SETUP ================//
template <class Transport>
void CKC_PnP<Transport>::setupAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
    WiFi.softAP(_ap_ssid, _ap_pass);

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());

    setupWeb();
}

//================ TASK STA ================//
template <class Transport>
void CKC_PnP<Transport>::TASK_STA()
{
    static bool started = false;

    if (!started)
    {
        Serial.println("TASK STA");
        started = true;

        WiFi.mode(WIFI_STA);

        if (strlen(_sta_ssid) > 0)
            WiFi.begin(_sta_ssid, _sta_pass);
        else
            connectMultiWiFi();

        t0 = millis();
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected STA");
        WiFi_TASK = MODE_CONNECTED;
        started = false;
        return;
    }

    if (millis() - t0 > time_sta)
    {
        Serial.println("STA FAIL -> AP");
        WiFi_TASK = MODE_AP;
        started = false;
    }
}

//================ TASK AP ================//
template <class Transport>
void CKC_PnP<Transport>::TASK_AP()
{
    static bool started = false;

    if (!started)
    {
        Serial.println("TASK AP");
        setupAP();
        started = true;
    }

    server->handleClient();
}

//================ TASK CONNECTED ================//
template <class Transport>
void CKC_PnP<Transport>::TASK_CONNECTED()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Lost WiFi");
        WiFi_TASK = MODE_STA;
        return;
    }

    static unsigned long t = 0;
    if (millis() - t > 5000)
    {
        t = millis();
        Serial.println(WiFi.localIP());
    }
}

//================ TASK AP+STA ================//
template <class Transport>
void CKC_PnP<Transport>::TASK_AP_STA()
{
    WiFi.mode(WIFI_AP_STA);
    server->handleClient();
}

//================ BUTTON ================//
template <class Transport>
void CKC_PnP<Transport>::handler_button()
{
    bool pressed = digitalRead(FLASH_BTN) == LOW;

    if (pressed)
    {
        if (pressStart == 0)
            pressStart = millis();

        if (!triggered && millis() - pressStart > 5000)
        {
            triggered = true;
            Serial.println("FORCE AP MODE");
            WiFi_TASK = MODE_AP;
        }
    }
    else
    {
        pressStart = 0;
        triggered = false;
    }
}

//================ RUN ================//
template <class Transport>
void CKC_PnP<Transport>::run()
{
    switch (WiFi_TASK)
    {
    case MODE_STA:
        TASK_STA();
        break;

    case MODE_AP:
        TASK_AP();
        break;

    case MODE_CONNECTED:
        TASK_CONNECTED();
        break;

    case MODE_AP_STA:
        TASK_AP_STA();
        break;
    }

    handler_button();
}

#endif