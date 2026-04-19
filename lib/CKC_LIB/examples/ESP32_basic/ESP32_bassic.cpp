#include <Arduino.h>
#define CKC_DEBUG
#define BUTTON_MODE

const char *SSID = "CKC";
const char *PASS = "CKC2026";
#include <CKC.h>
CKC_WRITE(V3)
{
  int a = param.getInt();
  CKC_LOG_DEBUG("CKC", "value%d", a);
}
CKC_WRITE(V2)
{
  int a = param.getFloat();
  CKC_LOG_DEBUG("CKC1", "value%f", a);
}
void setup()
{
  Serial.begin(115200);
  pinMode(26, OUTPUT);
  CKC.begin(SSID, PASS);
}
void loop()
{
  CKC.run();
  if (CKC.connected())
  {
    digitalWrite(26, 1);
  }
  else
  {
    digitalWrite(26, 0);
  }
}





// #include <Arduino.h>
// // #define PLGtoken "PLG2026"
// #define CKC_DEBUG
// #define BUTTON_MODE
// const char *SSID = "MakerSpaceLab_5.0Ghz";
// const char *PASS = "Maker2025";
// #define NTC_pin 36
// float doccambien()
// {
//   float adc = 0.0;
//   float adc_max = 4095.0;
//   float Vref = 3.3;
//   float Vout = 0.0;

//   float Rcd = 750.0; // Điện trở cố định được đấu trên mạch
//   float RNTCchuaxuly = 0.0;

//   float RBase = 10000.0;       // Điện trở mà NTC cho ra được khi ở 25°C // Theo DataSheet của cảm biến
//   float TBASE = 25.0 + 273.15; // Nhiệt độ 25°C // Theo DataSheet của cảm biến
//   float tongRNTC = 0.0;
//   float Beta = 3950.0; // Theo DataSheet của cảm biến

//   for (int i = 0; i < 100; i++)
//   {
//     adc = analogRead(NTC_pin);
//     RNTCchuaxuly = ((adc * Rcd) / (adc_max - adc));
//     tongRNTC += RNTCchuaxuly;
//     delay(10);
//   }
//   float RNTC = (tongRNTC / 100.0) + 2.242;
//   if (RNTC < 0.174)
//     RNTC = 0.174; // Theo DataSheet của cảm biến
//   float T = (Beta * TBASE) / (((-log(RBase) + log(RNTC)) * TBASE) + Beta);
//   float tempValue = T - 273.15;
//   return tempValue;
// }

// #include <CKC.h>
// void setup()
// {
//   Serial.begin(115200);
//   CKC.begin(SSID, PASS);
// }
// void loop()
// {
//   CKC.run();
//   static unsigned long PLGTimer = 0;
//   if (millis() - PLGTimer > 1000)
//   {
//     float temp = doccambien();
//     // CKC.WriteTelemetry();
//   }
// }