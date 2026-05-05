#include <Arduino.h>
#include <khaibao.h>
#define CKC_DEBUG
#define BUTTON_MODE
int RL[4] = {RL1, RL2, RL3, RL4};

unsigned long time_P = 0;
#include <CKC.h>
const char *SSID = "Homi";
const char *PASS = "12345689";

#define NTC_pin 36
const unsigned long SampleTime = 10;
unsigned long timer = 0;
int Sum_adc = 0;
int count = 0;

float SensorNTC()
{
  static float lastTemp = 0.0;
  // thông số mạch
  float VCC = 3.3;
  float ADC_MAX = 4095.0;
  float R_Fixed = 9680.0;
  // thông số NTC
  float Beta = 3950.0;
  float RBase = 10000.0;
  float TBase = (273.15 + 25.0); // Nhiệt độ Keven khi ở 25°C
  float RNTC = 0.0;
  if (millis() - timer > SampleTime)
  {
    timer = millis();
    count++;
    int adc = analogRead(NTC_pin);
    Sum_adc += adc;
    if (count >= 100)
    {
      float adcAVG = Sum_adc / 100.0; // tính trung bình cộng ADC
      float Vadc = ((adcAVG * VCC) / ADC_MAX);
      count = 0;
      Sum_adc = 0;
      adcAVG = 0;
      if ((Vadc < 0.001) || (Vadc > 3.29))
      {
        Serial.println("Out of Range");
        count = 0;
        Sum_adc = 0;
        adcAVG = 0;
        return 0; // lỗi do ngõ vào ADC bị nhiễu nặng hoặc mất tính hiệu
      }
      // Tính toán điện trở và nhiệt độ
      RNTC = R_Fixed * Vadc / (VCC - Vadc);
      RNTC = RNTC * 1.25; // scale giá trị để gần đúng với thực tế
      float tempK = 1.0 / (1.0 / TBase + (1.0 / Beta) * log(RNTC / RBase));
      float tempC = tempK - 273.15;
      lastTemp = tempC;
    }
  }
  return lastTemp;
}

#define VCC_Pin 13
#define LED 2
void setup()
{
  Serial.begin(115200);
  pinMode(NTC_pin, INPUT);
  pinMode(VCC_Pin, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(VCC_Pin, HIGH);
  CKCModbus.beginModbus(Serial2, 9600, RX1, TX1);
  CKCModbus.setTimeout(1000);

  CKC.begin(SSID, PASS);
  CKC.set_Telemetry("TEMP", NULL);
  for (int i = 0; i < 4; i++)
  {
    pinMode(RL[i], OUTPUT);
    digitalWrite(RL[i], LOW);
  }
}
void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }
  // float voltage = CKCModbus.readFloat(3, 0x14); // modbus
  // float frequency = CKCModbus.readFloat(3, 0x1A);
  // float current = CKCModbus.readFloat(3, 0x16);
  // float Temp = SensorNTC();
  CKC.run();
  if (millis() - time_P > 3000)
  {
    time_P = millis();
    // CKC.WriteTelemetry("TEMP", String(Temp, 2));
  }
}


// #include <Arduino.h>
// #include "nvs_flash.h"
// void setup()
// {
//   Serial.begin(115200);
//   nvs_flash_erase(); // xóa toàn bộ flash NVS
//   nvs_flash_init();  // khởi tạo lại
//   Serial.println("Da xoa toan bo flash!");
// }
// void loop()
// {
// }