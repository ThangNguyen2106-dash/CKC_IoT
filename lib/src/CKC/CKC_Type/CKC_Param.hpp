#ifndef INC_CKC_Param
#define INC_CKC_Param
#include <stdio.h>
#include <stdint.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <CKC/CKC_topic.h>

#pragma
class CKCParam
{
public:
    enum class Type : uint8_t
    {
        NONE = 0,
        INT,
        FLOAT,
        BOOL,
        STRING
    };

    CKCParam() : type(Type::NONE) {}
    void add(int v) { intVal = v; }
    // Constructors
    CKCParam(int v) { set(v); }
    CKCParam(float v) { set(v); }
    CKCParam(bool v) { set(v); }
    CKCParam(const char *v) { set(v); }
    CKCParam(const String &v) { set(v); }

    void set(int v)
    {
        type = Type::INT;
        data.i = v;
    }

    void set(float v)
    {
        type = Type::FLOAT;
        data.f = v;
    }

    void set(bool v)
    {
        type = Type::BOOL;
        data.b = v;
    }

    void set(const char *v)
    {
        type = Type::STRING;
        str = v;
    }

    void set(const String &v)
    {
        type = Type::STRING;
        str = v;
    }

    // get type data
    Type getType() const { return type; }

    int getInt(int def = 0) const
    {
        if (type == Type::INT)
            return data.i;
        if (type == Type::BOOL)
            return data.b ? 1 : 0;
        if (type == Type::FLOAT)
            return (int)data.f;
        if (type == Type::STRING)
            return str.toInt();
        return def;
    }

    float getFloat(float def = 0) const
    {
        if (type == Type::FLOAT)
            return data.f;
        if (type == Type::INT)
            return (float)data.i;
        if (type == Type::BOOL)
            return data.b ? 1.0f : 0.0f;
        if (type == Type::STRING)
            return str.toFloat();
        return def;
    }

    bool getBool(bool def = false) const
    {
        if (type == Type::BOOL)
            return data.b;
        if (type == Type::INT)
            return data.i != 0;
        if (type == Type::FLOAT)
            return data.f != 0;
        if (type == Type::STRING)
            return (str == "1" || str == "true" || str == "TRUE");
        return def;
    }

    String getString(const String &def = "") const
    {
        if (type == Type::STRING)
            return str;
        if (type == Type::INT)
            return String(data.i);
        if (type == Type::FLOAT)
            return String(data.f);
        if (type == Type::BOOL)
            return data.b ? "true" : "false";
        return def;
    }

    // add SUB_PREFIX after parse
    void addSUB_PREFIX(String subprefix_H)
    {

        this->subprefix_h = subprefix_H;
    }
    // add payload after parse
    void addpin_(int Pin_H)
    {
        this->Pin_h = Pin_H;
    }
    void addtype_(String type_H)
    {
        this->type_h = type_H;
    }
    void addvalue_(int value_H)
    {
        this->value_h = value_H;
    }
    // get payload after parse
    int getvalue_()
    {
        return value_h;
    }
    int getpin_()
    {
        return Pin_h;
    }
    String gettype_()
    {
        return type_h;
    }
    String getsubprefix()
    {
        return subprefix_h;
    }

private:
    Type type;
    union
    {
        int i;
        float f;
        bool b;
    } data;
    int intVal;
    String str;

    String type_h, subprefix_h;
    int value_h, Pin_h;
};
CKCParam PARSE;
//==================== Tách dữ liệu ===================//
inline void CKC_PARSE(const char *topic, const char *payload)
{
    if (strncmp(topic, CKC_BASE_TOPIC, strlen(CKC_BASE_TOPIC)) != 0)
        return;
    String topicStr = topic;

    String payloadStr = payload;

    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, payloadStr);
    if (error)
    {
        Serial.println("JSON lỗi");
        return;
    }

    String base_Topic;
    String token;
    String sub_Prefix;

    int p1 = topicStr.indexOf('/');
    int p2 = topicStr.indexOf('/', p1 + 1);
    int p3 = topicStr.indexOf('/', p2 + 1);
    int p4 = topicStr.indexOf('/', p3 + 1);

    base_Topic = topicStr.substring(0, p2);
    token = topicStr.substring(p2 + 1, p3);
    sub_Prefix = topicStr.substring(p3 + 1, p4);

    int pin_ = doc["GPIO"];
    int value_ = doc["value"];
    String type_ = doc["type"];

    Serial.println("==============Tách chuỗi==================");
    Serial.print("Base topic: ");
    Serial.println(base_Topic);

    Serial.print("Token: ");
    Serial.println(token);

    Serial.print("SUB_PREFIX: ");
    Serial.println(sub_Prefix);

    Serial.print("GPIO: ");
    Serial.println(pin_);

    Serial.print("TYPE: ");
    Serial.println(type_);

    Serial.print("VALUE: ");
    Serial.println(value_);

    PARSE.addpin_(pin_);
    PARSE.addtype_(type_);
    PARSE.addvalue_(value_);
    PARSE.addSUB_PREFIX(sub_Prefix);
}
//==================== Xử lý dữ liệu ===================//
void CKC_PARAM()
{
    String type = PARSE.gettype_();
    int value = PARSE.getvalue_();
    int pin = PARSE.getpin_();
    String sub_Prefix = PARSE.getsubprefix();
    if (sub_Prefix == "arduino_pin")
    {
        if (type == "DI") // Digital Input
        {
            pinMode(pin, INPUT);
        }
        else if (type == "AI") // Analog Input
        {
            pinMode(pin, INPUT);
        }

        else if (type == "DO") // Digital OUTPUT
        {
            pinMode(pin, OUTPUT);
            CKCParam param(value);
            digitalWrite(pin, value);
        }
        else if (type == "AO") // Analog OUTPUT
        {
            pinMode(pin, OUTPUT);
            CKCParam param(value);
            analogWrite(pin, value);
        }
    }
    if (sub_Prefix == "virtual_pin")
    {
        Serial.println("Nhận lệnh điều khiển chân ảo");
    }
}
#endif