#include <CKC_src/CKC_Source/CKC_Class.hpp>
#include <CKC_src/CKC_Source/PubSub_Client/PubSubClient.h>
#include <stdint.h>
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    String msg;
    for (int i = 0; i < length; i++)
        msg += (char)payload[i];
    Serial.print("[MQTT] ");
    Serial.print(topic);
    Serial.print(" => ");
    Serial.println(msg);

    if (String(topic) == "led/2")
    {
        int value = msg.toInt(); // "1" / "0"
        CKC_Virtual.write(1, value);
    }
}
WiFiClientSecure server;
PubSubClient mqttClient(server);
CKC_MQTT CKC_IoT_MQTT;
void CKC_MQTT::begin(String WiFiID,
                     String PassWiFi,
                     String MQTT_server,
                     uint16_t MQTT_Port,
                     String MQTT_ID,
                     String MQTT_user,
                     String MQTT_pass) // Hàm cấu hình Wifi và set up MQTT Server
{
    ssidSTA = WiFiID;
    passSTA = PassWiFi;
    mqttServer = MQTT_server;
    mqttPort = MQTT_Port;
    mqttid = MQTT_ID;
    mqttUser = MQTT_user;
    mqttPassword = MQTT_pass;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
    Serial.print("[CKC] WiFi connecting");
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime <= 10000)
    {
        delay(1000);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        Serial.println("[CKC] Connected! ESP32 IP: " + WiFi.localIP().toString());
        server.setInsecure();                               // hàm cài đặt bỏ qua bước xác thực
        mqttClient.setServer(mqttServer.c_str(), mqttPort); // set up MQTT Server với link Server và cổng PORT
        Serial.print("[CKC] MQTT connecting...");
        if (mqttClient.connect(mqttid.c_str(), mqttUser.c_str(), mqttPassword.c_str()))
        {
            Serial.println("OK");
            mqttClient.setCallback(mqttCallback);
        }
        else
        {
            Serial.print("FAILED, rc=");
            Serial.println(mqttClient.state());
        }
    }
    else
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("ESP-AP MODE", "123456789", 1);
        IPAddress IP = WiFi.softAPIP();
        Serial.println("\n[CKC] CAN'T CONNECT TO WiFI");
        Serial.println("[CKC] SWITCHING TO AP MODE");
        Serial.println("[CKC] ESP32 AP IP address: " + IP.toString());
        Serial.println("[CKC] SSID: ESP-AP MODE");
        Serial.println("[CKC] Pass: 123456789");
    }
}

void CKC_MQTT::run() // hàm hoạt động của MQTT
{
    if (mqttClient.connected())
        mqttClient.loop();
}

void CKC_MQTT::sendData(String Topic_s, String Data) // gửi dữ liệu kèm theo TOPIC đến MQTT Server, đồng thời kiểm tra kết nỗi trước khi gửi
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[CKC] WiFi lost");
        return;
    }
    if (!mqttClient.connected())
    {
        Serial.print("[CKC] MQTT reconnect...");
        if (!mqttClient.connect(mqttid.c_str(), mqttUser.c_str(), mqttPassword.c_str()))
        {
            Serial.println("FAILED");
            return;
        }
        Serial.println("OK");
    }
    mqttClient.publish(Topic_s.c_str(), Data.c_str());
    Serial.println("[CKC] Sent: " + Topic_s + " -> " + Data);
}