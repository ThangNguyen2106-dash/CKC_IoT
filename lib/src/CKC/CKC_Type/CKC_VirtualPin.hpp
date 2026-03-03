#include <stdio.h>
#include <stdint.h>
#include <Arduino.h>
#include <MQTT/ESP32_MQTT.hpp>
#ifndef CKC_VIRTUALPIN_HPP
#define CKC_VIRTUALPIN_HPP
#define MAX_VIRTUAL_PIN 100
class CKC_Virtualpin
{
public:
    typedef void (*CKC_VP_CALLBACK)(uint8_t &, const CKCParam &);

    CKC_Virtualpin();

    void attach(uint8_t pin, CKC_VP_CALLBACK cb);
    void run(uint8_t pin, const CKCParam &param);

private:
    CKC_VP_CALLBACK vpCallback[MAX_VIRTUAL_PIN];
};
#endif
CKC_Virtualpin CKC_Virtual;
CKC_Virtualpin::CKC_Virtualpin()
{
    for (int i = 0; i < MAX_VIRTUAL_PIN; i++)
        vpCallback[i] = nullptr;
}

void CKC_Virtualpin::attach(uint8_t pin, CKC_VP_CALLBACK cb)
{
    if (pin < MAX_VIRTUAL_PIN)
        vpCallback[pin] = cb;
}

void CKC_Virtualpin::run(uint8_t pin, const CKCParam &param)
{
    if (pin < MAX_VIRTUAL_PIN && vpCallback[pin] != nullptr)
        vpCallback[pin](pin, param);
}