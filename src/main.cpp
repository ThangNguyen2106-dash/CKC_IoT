#include <Arduino.h>
// #define PLGtoken "PLG2026"
#define CKC_DEBUG
#define BUTTON_MODE
const char *SSID = "";
const char *PASS = "";
#include <CKC.h>

void setup()
{
  Serial.begin(115200);
  CKC.init(SSID, PASS);
}
void loop()
{
  CKC.run();
  static unsigned long PLGTimer = 0;
  if (millis() - PLGTimer > 2000)
  {
    char DATA_[20];
    float temp = random(300, 400) / 10.0;
    snprintf(DATA_, sizeof(DATA_), "%.1f", temp);
    PLGTimer = millis();
    serverMQTT.sendData("Topic", DATA_);
  }
}
