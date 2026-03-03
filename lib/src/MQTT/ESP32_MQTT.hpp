#ifndef CKC_ESP32_MQTT_HPP
#define CKC_ESP32_MQTT_HPP
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <MQTT/NPT_Client/NTPClient.h>
#include <MQTT/PubSubClient/PubSubClient.h>
#include <CKC/CKC_Type/CKC_PIN.hpp>
#include <CKC/CKC_Type/CKC_VirtualPin.hpp>
#include <stdint.h>

const char *MQTT_Server = "1e5f657dbd934df698af2d814b1fce1a.s1.eu.hivemq.cloud";
const int16_t MQTT_PORT = 8883;

const char *MQTT_ID = "1e5f657dbd934df698af2d814b1fce1a";
const char *MQTT_USERNAME = "ThangNguyen2106";
const char *MQTT_PASS = "TxT21062005";

class CKC_MQTT
{
public:
    void begin();
    void run();
    void sendData(String Topic_s, String Data);
    void receiveData(String Topic_r);

private:
    String Data_receive;
};

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (int i = 0; i < length; i++)
        message += (char)payload[i];

    Serial.print("[MQTT] ");
    Serial.print(topic);
    Serial.print(" => ");
    Serial.println(message);
    CKCParam param(message);
    if (strcmp(topic, "V1") == 0)
    {
        CKC_Virtual.run(V1, param);
    }
    else if (strcmp(topic, "V2") == 0)
    {
        CKC_Virtual.run(V2, param);
    }
    else if (strcmp(topic, "V3") == 0)
    {
        CKC_Virtual.run(V3, param);
    }
}

WiFiClientSecure server;
PubSubClient mqttClient(server);

void CKC_MQTT::begin() // Hàm set up MQTT Server
{
    server.setInsecure();                         // hàm cài đặt bỏ qua bước xác thực
    mqttClient.setServer(MQTT_Server, MQTT_PORT); // set up MQTT Server với link Server và cổng PORT
    Serial.print("[CKC] MQTT connecting...");
    if (mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
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

void CKC_MQTT::run() // hàm hoạt động của MQTT
{
    if (mqttClient.connected())
        mqttClient.loop();
}

void CKC_MQTT::sendData(String Topic_s, String Data) // gửi dữ liệu kèm theo TOPIC đến MQTT Server, đồng thời kiểm tra kết nỗi trước khi gửi
{
    if (!mqttClient.connected())
    {
        Serial.print("[CKC] MQTT reconnect...");

        if (!mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
        {
            Serial.println("FAILED");
        CKC_MQTT:
            begin();
            return;
        }
        Serial.println("OK");
    }
    mqttClient.publish(Topic_s.c_str(), Data.c_str());
    Serial.println("[CKC] Sent: " + Topic_s + " -> " + Data);
}

#endif // END CKC_ESP32_MQTT_HPP