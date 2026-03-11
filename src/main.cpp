#include <Arduino.h>
#define PLGtoken "PLG2026"
#define CKC_DEBUG
#define BUTTON_MODE
const char *SSID = "MakerSpaceLab_2.4Ghz";
const char *PASS = "Maker2025";
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
    PLGTimer = millis();
  }
}
