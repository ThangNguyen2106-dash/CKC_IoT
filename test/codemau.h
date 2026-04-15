// #ifndef CKC_ESP32_MQTT_HPP
// #define CKC_ESP32_MQTT_HPP

// #include <Arduino.h>
// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <WiFiClientSecure.h>
// #include <WiFiUdp.h>
// #include <MQTT/NPT_Client/NTPClient.h>
// #include <MQTT/PubSubClient/PubSubClient.h>
// #include <stdint.h>
// #include <CKC/CKC_Param.hpp>
// #include "CKC/CKC_API.hpp"

// // #define CKC_MQTT_BASE_TOPIC CKC_BASE_TOPIC

// template <class MQTT>
// class CKC_MQTT
// {
// public:
//     void begin();
//     void run();
//     void sendData(String Topic_s, String Data);
//     void receiveData(String Topic_r);
//     bool _connect();
//     void CKC_subscribeTopic(const char *baseTopic, const char *Topic_ne);
//     void CKC_unsubscribeTopic(const char *baseTopic, const char *Topic_ne);
//     void CKC_publishTopic(const char *baseTopic, const char *Topic_ne);
//     void CKC_unpublishTopic(const char *baseTopic, const char *Topic_ne);
//     void CKC_publishData(const char *data);
//     bool check_mode_sub(char *topic, char *mess);

// private:
//     const char *MQTT_Server = "mqtt.ckc.kthd.vn"; // PORT MQTT 1883 PORT SSL 8883
//     const int16_t MQTT_PORT = 8883;
//     const char *MQTT_ID = "6937adf7e70d48879245c5a2b4299e63";
//     const char *MQTT_USERNAME = "";
//     const char *MQTT_PASS = "";
//     char CKC_MQTT_BASE_TOPIC[30] = CKC_BASE_TOPIC;
//     char _mac[12];
// };

// WiFiClientSecure server;
// PubSubClient mqttClient(server);
// CKC_MQTT<PubSubClient> mqtt;

// void CKC_Callback(char *topic, byte *payload, unsigned int length)
// {
//     char *msg = (char *)malloc(length + 1);
//     if (!msg)
//         return;
//     memcpy(msg, payload, length);
//     msg[length] = '\0';
//     API_MESS.handleMessage(topic, msg);
//     free(msg);
// }

// template <class MQTT>
// inline void CKC_MQTT<MQTT>::CKC_subscribeTopic(const char *baseTopic, const char *Topic_ne)
// {
//     char NameTopic[100];
//     snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
//     mqttClient.subscribe(NameTopic);
// }

// template <class MQTT>
// inline void CKC_MQTT<MQTT>::CKC_unsubscribeTopic(const char *baseTopic, const char *Topic_ne)
// {
//     char NameTopic[100];
//     snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
//     mqttClient.unsubscribe(NameTopic);
// }

// /* Publish Topic */
// template <class MQTT>
// inline void CKC_MQTT<MQTT>::CKC_publishTopic(const char *baseTopic, const char *Topic_ne)
// {
//     char NameTopic[100];
//     snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
// }

// template <class MQTT>
// inline void CKC_MQTT<MQTT>::CKC_unpublishTopic(const char *baseTopic, const char *Topic_ne)
// {
// }

// template <class MQTT>
// inline void CKC_MQTT<MQTT>::CKC_publishData(const char *data)
// {
//     // this ->CKC_publishTopic(CKC_MQTT_BASE_TOPIC,CKC_PUB_PREFIX_INFO_TOPIC);
//     char NameTopic[100];
//     snprintf(NameTopic, sizeof(NameTopic), "%s%s", CKC_MQTT_BASE_TOPIC, CKC_PUB_PREFIX_INFO_TOPIC);
//     mqttClient.publish(NameTopic, data);
// }

// template <class MQTT>
// inline void CKC_MQTT<MQTT>::begin()
// {
//     if (WiFi.status() != WL_CONNECTED)
//         return;
//     server.setInsecure();
//     mqttClient.setServer(MQTT_Server, MQTT_PORT);
//     CKC_LOG_DEBUG("MQTT", "Connecting_________");
//     if (mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
//     {
//         CKC_LOG_DEBUG("MQTT", "OK");
//         mqttClient.setCallback(CKC_Callback);
//         String MAC = WiFi.macAddress();
//         strcpy(_mac, MAC.c_str());
//         snprintf(CKC_MQTT_BASE_TOPIC, sizeof(CKC_MQTT_BASE_TOPIC), "%s%s", CKC_MQTT_BASE_TOPIC, _mac);
//         this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_ARDUINO_TOPIC);
//         this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_VIRTUAL_TOPIC);
//         this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_ASK_WIFI_TOPIC);
//         this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_CHANGE_WIFI_TOPIC);
//     }
//     else
//     {
//         CKC_LOG_DEBUG("MQTT", "FAILED, rc=", mqttClient.state());
//     }
// }

// template <class MQTT>
// inline bool CKC_MQTT<MQTT>::_connect()
// {

//     if (WiFi.status() != WL_CONNECTED)
//     {
//         return false;
//     }
//     return mqttClient.connected();
// }

// template <class MQTT>
// inline void CKC_MQTT<MQTT>::run()
// {
//     if (WiFi.status() != WL_CONNECTED)
//         return;
//     if (!mqttClient.connected())
//     {
//         Serial.print("[CKC] MQTT reconnect...");

//         if (mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
//         {
//             Serial.println("OK");
//             mqttClient.setCallback(CKC_Callback);
//         }
//         else
//         {
//             Serial.println("FAILED");
//             return;
//         }
//     }
//     mqttClient.loop();
//     // static unsigned long PLGTimer = 0;
//     // if (millis() - PLGTimer > 2000)
//     // {
//     //     PLGTimer = millis();
//     //     this->CKC_publishData("dasdasd");
//     // }
// }

// template <class MQTT>
// inline void CKC_MQTT<MQTT>::sendData(String Topic_s, String Data)
// {
//     if (WiFi.status() != WL_CONNECTED)
//         return;
//     if (!mqttClient.connected())
//     {
//         Serial.print("[CKC] MQTT reconnect...");
//         if (!mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
//         {
//             Serial.println("FAILED");
//             return;
//         }

//         Serial.println("OK");
//     }
//     mqttClient.publish(Topic_s.c_str(), Data.c_str());
// }

// CKC_MQTT<PubSubClient> serverMQTT;

// #endif // END CKC_ESP32_MQTT_HPP



// #include <Preferences.h>

// Preferences prefs;

// void setup() {
//   Serial.begin(115200);

//   prefs.begin("wifi", false); // namespace "wifi"
//   prefs.clear();              // XÓA TOÀN BỘ
//   prefs.end();

//   Serial.println("Đã xóa Preferences!");
// }

// void loop() {}




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
#define AP_WIFI_IP "192.168.1.4"
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
    char _mac[12];
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
    bool apMode = false;
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
    apMode = false;
    loadWiFi();
    // ==============================
    //     ƯU TIÊN WIFI TỪ INIT()
    // ==============================
    if (strlen(_sta_ssid) > 0)
    {
        t1 = millis();
        WiFi.begin(_sta_ssid, _sta_pass);
        CKC_LOG_DEBUG("WIFI", "Connecting to: ", _sta_ssid);
        while (WiFi.status() != WL_CONNECTED && millis() - t1 <= this->time_sta)
        {
            if (millis() - t0 > 1000)
            {
                CKC_LOG_DEBUG("WIFI", "CONNECTING ___ %ds\r", (millis() - t1) / 1000);
                t0 = millis();
            }
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
            SaveWiFi(String(_sta_ssid), String(_sta_pass));
            delay(100);
            WiFi_TASK = MODE_CONNECTED;
            return;
        }
    }
    WiFi.mode(WIFI_STA);
    Serial.println("Scanning WiFi...");
    int n = WiFi.scanNetworks();
    if (n <= 0)
    {
        CKC_LOG_DEBUG("WIFI", "NO_WIFI_CONNECT !!!!!! ");
        delay(100);
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
                CKC_LOG_DEBUG("WIFI", "Connecting to: ", ssid_list[j]);
                t1 = millis();
                WiFi.begin(ssid_list[j].c_str(), pass_list[j].c_str());
                while (WiFi.status() != WL_CONNECTED && millis() - t1 <= this->time_sta)
                {
                    if (millis() - t0 > 1000)
                    {
                        CKC_LOG_DEBUG("WIFI", "CONNECTING ___ %ds\r", (millis() - t1) / 1000);
                        t0 = millis();
                    }
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
                    delay(100);
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
    delay(100);
    WiFi_TASK = MODE_AP;
}
//========== AP MODE ==========//
template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_AP()
{
    // serverMQTT.disconnect();
    CKC_LOG_DEBUG("WIFI", "RUN_AP");
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);
    apMode = true;
    WiFi.mode(WIFI_OFF);
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
    delay(100);
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
    if (apMode)
        return;
    if (WiFi.status() == WL_CONNECTED)
    {
        serverMQTT.run();

        if (!serverMQTT._connect())
        {
            serverMQTT.begin();
        }
    }
    else
    {
        WiFi_TASK = MODE_STA;
        t1 = millis();
    }
}

//================ BUTTON ================
template <class Transport>
inline void CKC_PnP<Transport>::handler_button()
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

        // nhả nút thì reset lại
        pressStart = 0;
        triggered = false;
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
        this->webServer.handleClient();
        break;
    default:
        break;
    }
    this->handler_button();
}

#endif

