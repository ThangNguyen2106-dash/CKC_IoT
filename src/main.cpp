#include <Arduino.h>
#include <khaibao.h>
#define CKC_DEBUG
#define BUTTON_MODE
int RL[4] = {RL1, RL2, RL3, RL4};
unsigned long time_P = 0;
#include <CKC.h>
const char *SSID = "";
const char *PASS = "";

CKC_WRITE(V1)
{
  int a = param.getInt();              // Lấy dữ liệu kiểu int
  CKC_LOG_DEBUG("CKC", "value %d", a); // In debug
  digitalWrite(RL1, a);
}
CKC_WRITE(V2)
{
  int a = param.getInt();              // Lấy dữ liệu kiểu int
  CKC_LOG_DEBUG("CKC", "value %d", a); // In debug
  digitalWrite(RL2, a);
}
CKC_WRITE(V3)
{
  int a = param.getInt();              // Lấy dữ liệu kiểu int
  CKC_LOG_DEBUG("CKC", "value %d", a); // In debug
  digitalWrite(RL3, a);
}
CKC_WRITE(V4)
{
  int a = param.getInt();              // Lấy dữ liệu kiểu int
  CKC_LOG_DEBUG("CKC", "value %d", a); // In debug
  digitalWrite(RL4, a);
}

void setup()
{
  Serial.begin(115200);
  CKC.begin(SSID, PASS);
  CKC.set_Telemetry("TEM", NULL);
  for (int i = 0; i < 4; i++)
  {
    pinMode(RL[i], OUTPUT);
    digitalWrite(RL[i], LOW);
  }
}
void loop()
{
  CKC.run();
  if (millis() - time_P > 1000)
  {
    time_P = millis();
    float temp = random(35.0, 37.0);
    // Gửi dữ liệu nhiệt độ lên server
    CKC.WriteTelemetry("TEM", temp);
  }
}
