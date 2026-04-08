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

#define WIFI_AP_IP IPAddress(192, 168, 27, 1)
#define WIFI_AP_Subnet IPAddress(255, 255, 255, 0)

char STA_WIFI_NAME[32];
char STA_WIFI_PASS[32];

#define STA_WIFI_PORT "80"

#define AP_WIFI_NAME "CKC:"
#define AP_WIFI_PASS "CKC@2026"
#define AP_WIFI_IP "192.168.1.4"
#define AP_WIFI_PORT "80"
enum CKC_WiFI_TASK
{
    MODE_CONFIG_WIFI,
    MODE_STA,
    MODE_AP,
    MODE_AP_STA,
    MODE_CONNECTED,
    WIFI_DISCONNECTED,
    START_APCKC,
};
#define WIFI_MAX 5 // WiFi tối đa mà ESP32 có thể nhận

CKC_WiFI_TASK WiFi_TASK = MODE_CONFIG_WIFI;
template <class Transport>
class CKC_PnP
{
public:
    CKC_PnP() {};
    void init(const char *sta_ssid, const char *sta_pass);
    void CKC_state_Connect_STA();
    void CKC_state_Connect_AP();
    void CKC_mode_WiFi_connected();
    void CKC_CONNECTION();

    void run();

    void handleSave();
    void SaveWiFi(String newSSID, String newPASS);
    void loadWiFi();
    bool connectMultiWiFi();
    void setup_APWeb_WiFi();
    void handler_button();
    void CKC_mode_Config();

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
    char _mac[12];
    // var local
    unsigned long _SendRssiTime;
    unsigned int count_wifiConnect;
    bool ap_started = false;
    unsigned long t0, t1, t2, t3, t4, t5, t6, t7;
    String _ping;
    int time_sta = 20000;
    int time_ap = 30000;
#define FLASH_BTN 0 // nút BOOT/FLASH trên ESP32 thường là GPIO0

    unsigned long pressStart = 0;
    bool triggered = false;

    WebServer *server = nullptr;

    Preferences prefs;
    String ssid_list[WIFI_MAX];
    String pass_list[WIFI_MAX];

    bool apMode = false;
};

//======================= xử lý WiFi khi bị mất kết nối và bị lỗi rớt mạng ==================================//
template <class Transport>
inline void CKC_PnP<Transport>::handleSave() // ĐƯa biến WiFi từ Web vào
{
    String newSSID = server->arg("ssid");
    String newPASS = server->arg("pass");
    SaveWiFi(newSSID, newPASS);
    server->send(200, "text/html", "Saved!");
    delay(2000);
    ESP.restart();
}
template <class Transport>
inline void CKC_PnP<Transport>::SaveWiFi(String newSSID, String newPASS) // Lưu WiFi từ Web vào ESP32
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
inline void CKC_PnP<Transport>::loadWiFi() // Tải WiFi từ ESP32
{
    prefs.begin("wifi", true);
    for (int i = 0; i < WIFI_MAX; i++)
    {
        ssid_list[i] = prefs.getString(("ssid" + String(i)).c_str(), "");
        pass_list[i] = prefs.getString(("pass" + String(i)).c_str(), "");
    }
    prefs.end();
}
template <class Transport>
inline bool CKC_PnP<Transport>::connectMultiWiFi() // kết nối với WiFi được tải về từ ESP32
{
    loadWiFi();
    WiFi.mode(WIFI_STA);
    for (int i = 0; i < WIFI_MAX; i++)
    {
        if (ssid_list[i] == "")
            continue;
        CKC_LOG_DEBUG("WiFi", "CONNECTING", "Try:" + ssid_list[i]);
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
template <class Transport>
inline void CKC_PnP<Transport>::setup_APWeb_WiFi() // Cài đặt Setup Server khi esp32 vào chế độ AP
{
    server->on("/", [this]()
               { server->send(200, "text/html", htmlPage()); });

    server->on("/save", [this]()
               { this->handleSave(); });

    server->begin();
}

//======================= Kiểm tra trạng thái xử lý WiFi các chế độ và xử lý các chế độ khi rơi vào các trường hợp Mạng ===========================//
template <class Transport>
inline void CKC_PnP<Transport>::CKC_mode_WiFi_connected() // Kiểm tra các chế độ đã được kết nối
{
    static bool once = false;
    if (!once)
    {
        once = true;

        CKC_LOG_DEBUG("WIFI", "CONNECTED");
        CKC_LOG_DEBUG("WIFI", "IP: %s", WiFi.localIP().toString().c_str());

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
    }
    serverMQTT.run();
    if (WiFi.status() != WL_CONNECTED)
    {
        CKC_LOG_DEBUG("WIFI", "LOST WIFI");

        once = false;
        WiFi_TASK = MODE_CONFIG_WIFI;
    }
}
template <class Transport>
inline void CKC_PnP<Transport>::CKC_CONNECTION() // Kiểm tra WiFi có được kết nối hay chưa
{
    WiFi_TASK = MODE_STA;
}

// ============= Các chế độ khởi tạo WiFi và chương trình các chế độ hoạt động ============//
template <class Transport>
inline void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass) // Triển khai WiFi
{
    server = new WebServer(80);
    strcpy(_sta_ssid, sta_ssid);
    strcpy(_sta_pass, sta_pass);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", _ap_ssid, _mac);
    //=======================================================================================================
    Serial.println("\n====== WIFI LIST Trong ESP32 ======");
    prefs.begin("wifi", true);

    for (int i = 0; i < WIFI_MAX; i++)
    {
        String ssid = prefs.getString(("ssid" + String(i)).c_str(), "");
        String pass = prefs.getString(("pass" + String(i)).c_str(), "");

        Serial.printf("Priority %d:\n", i);

        if (ssid != "")
        {
            Serial.printf("   SSID: %s\n", ssid.c_str());
            Serial.printf("   PASS: %s\n", pass.c_str());
        }
        else
        {
            Serial.println("   (empty)");
        }
    }
    prefs.end();
    Serial.println("==============================\n");
    //=======================================================================================================
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", _sta_ip);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);

    delay(100);
    t1 = millis();
    pinMode(FLASH_BTN, INPUT_PULLUP); // nút kéo xuống GND khi nhấn
    WiFi_TASK = MODE_CONFIG_WIFI;
}

template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_STA() // Setup ở chế độ STA
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
inline void CKC_PnP<Transport>::CKC_state_Connect_AP() // Setup ở chế độ AP
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
inline void CKC_PnP<Transport>::handler_button() // Hàm sử dụng nút nhấn
{
#ifdef BUTTON_MODE
    bool pressed = (digitalRead(FLASH_BTN) == LOW); // nhấn = LOW

    if (pressed)
    {
        if (pressStart == 0)
            pressStart = millis();
        // giữ đủ 5s và chỉ kích 1 lần
        if (!triggered && (millis() - pressStart >= 5000))
        {
            triggered = true;
            CKC_LOG_DEBUG("WIFI", "MODE_AP_run:");
            CKC_LOG_DEBUG("WIFI", "WIFI_CONNECT_FALSE !!!!!! ");
            CKC_LOG_DEBUG("WIFI", "RUN_AP");
            CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
            CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
            CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
            CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);
            WiFi_TASK = MODE_AP;
            WiFi.mode(WIFI_OFF);
            WiFi.mode(WIFI_AP);
            _ipAddr.fromString(_ap_ip);
            WiFi.softAP(_ap_ssid, _ap_pass);
            t2 = millis();
            // TODO: đặt lệnh bạn muốn ở đây
        }
    }
    else
    {
        pressStart = 0;
        triggered = false;
    }
#endif
}

template <class Transport>
inline void CKC_PnP<Transport>::CKC_mode_Config() {
    // Sử dụng khi cần mở rộng thêm
};

template <class Transport>
inline void CKC_PnP<Transport>::run()
{
    switch (WiFi_TASK)
    {
    case MODE_CONFIG_WIFI:
        this->CKC_CONNECTION();
        break;
    case MODE_STA:
        this->CKC_state_Connect_STA();
        break;
    case MODE_AP:
        this->CKC_state_Connect_AP();
        break;
    case MODE_CONNECTED:
        this->CKC_mode_WiFi_connected();
        break;
    default:
        break;
    }
    this->handler_button();
}

#endif
