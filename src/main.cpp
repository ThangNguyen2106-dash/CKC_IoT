#include <Arduino.h>
#include <khaibao.h>
#define CKC_DEBUG
#define BUTTON_MODE
int RL[4] = {RL1, RL2, RL3, RL4};
unsigned long time_P = 0;
#include <CKC.h>
const char *SSID = "MakerSpaceLab_2.4Ghz";
const char *PASS = "Maker2025";
void setup()
{
  Serial.begin(115200);
  CKC.begin(SSID, PASS);
  CKC.set_Telemetry("TEM", "HUM", NULL);
  for (int i = 0; i < 4; i++)
  {
    pinMode(RL[i], OUTPUT);
    digitalWrite(RL[i], LOW);
  }
}
void loop()
{
  CKC.run();
  if (millis() - time_P > 3000)
  {
    time_P = millis();
    float temp = random(30.0, 35.0);
    float hum = random(70, 80);
    // Gửi dữ liệu nhiệt độ lên server
    CKC.WriteTelemetry("TEM", temp);
    CKC.WriteTelemetry("HUM", hum);
  }
}
