#include <Arduino.h>

#define led1 5
#define led2 21
#define led3 22
#define PLGtoken "PLG2026"
#define CKC_DEBUG
#define BUTTON_MODE
const char *SSID = "MakerSpaceLab_2.4Ghz";
const char *PASS = "Maker2025";
#include <CKC.h>
void setup()
{
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  CKC.init(SSID, PASS);
}
void loop()
{
  CKC.run();
}
