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
  CKCModbus.beginModbus(Serial2, 9600, RX1, TX1);
  CKCModbus.setTimeout(1000);

  // CKC.begin(SSID, PASS);
  // CKC.set_Telemetry("TEM", "HUM", "POW", NULL);
  // for (int i = 0; i < 4; i++)
  // {
  //   pinMode(RL[i], OUTPUT);
  //   digitalWrite(RL[i], LOW);
  // }
}
void loop()
{
  float voltage = CKCModbus.readFloat(3, 0x14);
  float frequency = CKCModbus.readFloat(3, 0x1A);
  float current = CKCModbus.readFloat(3, 0x16);

  Serial.println("===== EM2M =====");

  Serial.print("Voltage: ");
  Serial.println(voltage);

  Serial.print("Frequency: ");
  Serial.println(frequency);

  Serial.print("Current: ");
  Serial.println(current);
  // CKC.run();
  // if (millis() - time_P > 3000)
  //   // {
  //   time_P = millis();
  // float temp = readTempNTC();
  // float hum = readHum();
  // float P = readVoltageEM2M();

  // Gửi dữ liệu nhiệt độ lên server
  // CKC.WriteTelemetry("TEM", temp);
  // CKC.WriteTelemetry("HUM", hum);
  // CKC.WriteTelemetry("POW", P);
}
