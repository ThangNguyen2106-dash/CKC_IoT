#ifndef CKC_ESP32_MQTT_HPP
#define CKC_ESP32_MQTT_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <MQTT/NPT_Client/NTPClient.h>
#include <MQTT/PubSubClient/PubSubClient.h>
#include <stdint.h>
#include <AIoT/CKC_Param.hpp>
#include "AIoT/CKC_API.hpp"

template <class MQTT>
class CKC_MQTT
{
public:
    void config(const char *mqtt_userName, const char *mqtt_pass);
    void begin();
    void run();
    void receiveData(String Topic_r);
    bool _connect();
    void CKC_subscribeTopic(const char *baseTopic, const char *Topic_ne);
    void CKC_unsubscribeTopic(const char *baseTopic, const char *Topic_ne);
    void CKC_publishTopic(const char *baseTopic, const char *Topic_ne);
    void CKC_unpublishTopic(const char *baseTopic, const char *Topic_ne);
    void CKC_publishData(const char *data);
    bool check_mode_sub(char *topic, char *mess);

private:
    const char *MQTT_Server = "mqtt.ait.caothang.edu.vn";
    const int16_t MQTT_PORT = 8883;
    char MQTT_ID[21];
    char MQTT_USERNAME[28];
    char MQTT_PASS[21];
    char CKC_MQTT_BASE_TOPIC[30] = CKC_BASE_TOPIC;
    char _mac[12];
};

WiFiClientSecure server;
PubSubClient mqttClient(server);
CKC_MQTT<PubSubClient> mqtt;

void CKC_Callback(char *topic, byte *payload, unsigned int length)
{
    char *msg = (char *)malloc(length + 1);
    if (!msg)
        return;
    memcpy(msg, payload, length);
    msg[length] = '\0';
    API_MESS.handleMessage(topic, msg);
    free(msg);
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_subscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.subscribe(NameTopic);
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_unsubscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.unsubscribe(NameTopic);
}

/* Publish Topic */
template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_publishTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_unpublishTopic(const char *baseTopic, const char *Topic_ne)
{
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_publishData(const char *data)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", CKC_BASE_TOPIC, CKC_PUB_PREFIX_TELEMETRY_TOPIC);
    mqttClient.publish(NameTopic, data);
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::config(const char *mqtt_userName, const char *mqtt_pass)
{
    strcpy(MQTT_USERNAME, mqtt_userName);
    strcpy(MQTT_PASS, mqtt_pass);
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    String clientID = "ESP_" + mac;
    strcpy(MQTT_ID, clientID.c_str());
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::begin()
{
    // kiểm tra WiFi trước
    if (WiFi.status() != WL_CONNECTED)
    {
        CKC_LOG_DEBUG("MQTT", "NO WIFI");
        return;
    }
    server.setInsecure();
    mqttClient.setServer(MQTT_Server, MQTT_PORT);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    CKC_LOG_DEBUG("MQTT", "CONNECTING ---> SERVER");
    if (mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
        if (mqttClient.connect(MQTT_ID))
        {
            CKC_LOG_DEBUG("MQTT", "CONNECTED ---> SERVER");
            mqttClient.setCallback(CKC_Callback);
            snprintf(CKC_MQTT_BASE_TOPIC, sizeof(CKC_MQTT_BASE_TOPIC), "%s%s", CKC_MQTT_BASE_TOPIC, _mac);
            this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_TELEMETRY_TOPIC);
            this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_CONTROL_TOPIC);
        }
        else
        {
            CKC_LOG_DEBUG("MQTT", "FAILED, rc=", mqttClient.state());
        }
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::run()
{
    if (this->_connect())
    {
        mqttClient.loop();
        if (!mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
        {
            Serial.println("FAILED");
            this->begin();
            return;
        }
    }
}

template <class MQTT>
inline bool CKC_MQTT<MQTT>::_connect()
{
    if (mqttClient.connected())
    {
        return true;
    }
    else
    {
        return false;
    }
}

CKC_MQTT<PubSubClient> serverMQTT;

#endif // END CKC_ESP32_MQTT_HPP