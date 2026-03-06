#ifndef CKC_VIRTUAL_PIN_H
#define CKC_VIRTUAL_PIN_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define CKC_MAX_PINS 20
#define CKC_BASE_TOPIC "ckc/"

class CKCParam
{
public:
    CKCParam() : intVal(0) {}

    void add(int v) { intVal = v; }
    int getInt() const { return intVal; }

private:
    int intVal;
};

// ===== Default handler =====
void CKCWidgetWrite(uint8_t pin, const CKCParam &param)
{
    Serial.printf("No handler for V%d\n", pin);
}

// ===== Weak alias macro =====
#define CKC_ON_WRITE(Pin)                                        \
    void CKCWidgetWrite##Pin(uint8_t pin, const CKCParam &param) \
        __attribute__((weak, alias("CKCWidgetWrite")))

// Tạo 0–19
CKC_ON_WRITE(0);
CKC_ON_WRITE(1);
CKC_ON_WRITE(2);
CKC_ON_WRITE(3);
CKC_ON_WRITE(4);
CKC_ON_WRITE(5);
CKC_ON_WRITE(6);
CKC_ON_WRITE(7);
CKC_ON_WRITE(8);
CKC_ON_WRITE(9);
CKC_ON_WRITE(10);
CKC_ON_WRITE(11);
CKC_ON_WRITE(12);
CKC_ON_WRITE(13);
CKC_ON_WRITE(14);
CKC_ON_WRITE(15);
CKC_ON_WRITE(16);
CKC_ON_WRITE(17);
CKC_ON_WRITE(18);
CKC_ON_WRITE(19);

typedef void (*CKCWriteHandler_t)(uint8_t, const CKCParam &);

static const CKCWriteHandler_t CKCHandlerVector[CKC_MAX_PINS] = {
    CKCWidgetWrite0, CKCWidgetWrite1, CKCWidgetWrite2, CKCWidgetWrite3, CKCWidgetWrite4,
    CKCWidgetWrite5, CKCWidgetWrite6, CKCWidgetWrite7, CKCWidgetWrite8, CKCWidgetWrite9,
    CKCWidgetWrite10, CKCWidgetWrite11, CKCWidgetWrite12, CKCWidgetWrite13, CKCWidgetWrite14,
    CKCWidgetWrite15, CKCWidgetWrite16, CKCWidgetWrite17, CKCWidgetWrite18, CKCWidgetWrite19};

// ===== Dispatcher =====
inline void CKC_CallWriteHandler(uint8_t pin, const CKCParam &param)
{
    if (pin >= CKC_MAX_PINS)
        return;
    CKCHandlerVector[pin](pin, param);
}

// ===== Macro user =====
#define CKC_WRITE_2(Pin) \
    void CKCWidgetWrite##Pin(uint8_t pin, const CKCParam &param)

#define CKC_WRITE(Pin) CKC_WRITE_2(Pin)

// ========================================================
//           Tách topic + parse JSON tự động
// ========================================================

inline void CKC_HandleMQTT(const char *topic, const char *payload)
{

    // 1️⃣ Kiểm tra base topic
    if (strncmp(topic, CKC_BASE_TOPIC, strlen(CKC_BASE_TOPIC)) != 0)
        return;

    // 2️⃣ Copy topic để tách
    char topicCopy[100];
    strncpy(topicCopy, topic, sizeof(topicCopy));
    topicCopy[sizeof(topicCopy) - 1] = '\0';

    // 3️⃣ Tách bằng strtok
    char *token = strtok(topicCopy, "/");
    int index = 0;
    uint8_t pin = 255;

    while (token != nullptr)
    {
        if (index == 2 && strcmp(token, "virtual_pin") == 0)
        {
            token = strtok(nullptr, "/");
            if (token != nullptr)
            {
                pin = atoi(token);
            }
            break;
        }
        token = strtok(nullptr, "/");
        index++;
    }

    if (pin == 255)
        return;

    // 4️⃣ Parse JSON
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
        return;
    if (!doc.containsKey("value"))
        return;

    int value = doc["value"];

    // 5️⃣ Gọi handler
    CKCParam param;
    param.add(value);

    CKC_CallWriteHandler(pin, param);
}

#endif

// CKC_WRITE(5)
// {
//     int value = param.getInt();
//     Serial.printf("V5 received: %d\n", value);
//     digitalWrite(2, value);
// }