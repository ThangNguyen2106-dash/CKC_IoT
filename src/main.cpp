#include <Arduino.h>
// #define PLGtoken "PLG2026"
#define CKC_DEBUG
#define BUTTON_MODE
const char *SSID = "MakerSpaceLab_5.0Ghz";
const char *PASS = "Maker2025";
#define NTC_pin 36
float doccambien()
{
  float adc = 0.0;
  float adc_max = 4095.0;
  float Vref = 3.3;
  float Vout = 0.0;

  float Rcd = 10000.0; // Điện trở cố định được đấu trên mạch
  float RNTCchuaxuly = 0.0;

  float RBase = 10000.0;       // Điện trở mà NTC cho ra được khi ở 25°C // Theo DataSheet của cảm biến
  float TBASE = 25.0 + 273.15; // Nhiệt độ 25°C // Theo DataSheet của cảm biến
  float tongRNTC = 0.0;
  float Beta = 3950.0; // Theo DataSheet của cảm biến

  for (int i = 0; i < 100; i++)
  {
    adc = analogRead(NTC_pin);
    RNTCchuaxuly = ((adc * Rcd) / (adc_max - adc));
    tongRNTC += RNTCchuaxuly;
    delay(10);
  }
  float RNTC = (tongRNTC / 100.0) + 2.242;
  if (RNTC < 0.174)
    RNTC = 0.174; // Theo DataSheet của cảm biến
  float T = (Beta * TBASE) / (((-log(RBase) + log(RNTC)) * TBASE) + Beta);
  float tempValue = T - 273.15;
  return tempValue;
}

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
    float temp = doccambien();
    snprintf(DATA_, sizeof(DATA_), "%.1f", temp);
    PLGTimer = millis();
    serverMQTT.sendData("Topic", DATA_);
  }
}

// #include <Arduino.h>
// #include <math.h>
// #define CKC_DEBUG
// #include <CKC.h>
// #define NTC_pin 36

// const char *SSID = "MakerSpaceLab_2Ghz";
// const char *PASS = "Maker2025";

// float doccambien()
// {
//   float adc = 0.0;
//   float adc_max = 4095.0;
//   float Vref = 3.3;
//   float Vout = 0.0;

//   float Rcd = 10000.0; // Điện trở cố định được đấu trên mạch // Theo DataSheet của cảm biến
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

//   // offset RNTC

//   float RNTC = (tongRNTC / 100.0) + 514.511;
//   if (RNTC < 0.174)
//     RNTC = 0.174; // Theo DataSheet của cảm biến
//   // Serial.println("Điện trở: " + String(RNTC) + "Ohm");
//   float T = (Beta * TBASE) / (((-log(RBase) + log(RNTC)) * TBASE) + Beta);
//   float tempValue = T - 273.15;
//   return tempValue;
// }

// void setup()
// {
//   CKC.init(SSID, PASS);
//   Serial.begin(115200);
//   pinMode(NTC_pin, INPUT);
// }
// void loop()
// {
//   CKC.run();
//   float temp = doccambien();
//   Serial.println("Nhiệt độ: " + String(temp) + "°C");
//   serverMQTT.sendData("Topic", String(temp));
// }