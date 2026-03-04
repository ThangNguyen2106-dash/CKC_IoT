#include <Arduino.h>
#define led1 5
#define led2 21
#define led3 22
#define led 2
#define CKC_DEBUG
#define BUTTON_MODE
const char *SSID = "MakerSpaceLab_2.4Ghz";
const char *PASS = "Maker2025";
#include <CKC.h>

// CKC_WRITE(V1)
// {
//   if (param.getBool())
//   {
//     digitalWrite(led1, HIGH);
//     Serial.println("Đèn sáng");
//   }
//   else
//   {
//     digitalWrite(led1, LOW);
//     Serial.println("Đèn tắt");
//   }
// }
// CKC_WRITE(V2)
// {
//   if (param.getBool())
//   {
//     digitalWrite(led2, HIGH);
//     Serial.println("Đèn sáng");
//   }
//   else
//   {
//     digitalWrite(led2, LOW);
//     Serial.println("Đèn tắt");
//   }
// }
// CKC_WRITE(V3)
// {
//   if (param.getBool())
//   {
//     digitalWrite(led3, HIGH);
//     Serial.println("Đèn sáng");
//   }
//   else
//   {
//     digitalWrite(led3, LOW);
//     Serial.println("Đèn tắt");
//   }
// }

void setup()
{
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led, OUTPUT);
  CKC.init(SSID, PASS);
  // CKC_Virtual.attach(V1, CKC_WidgetWriteV1);
  // CKC_Virtual.attach(V2, CKC_WidgetWriteV2);
  // CKC_Virtual.attach(V3, CKC_WidgetWriteV3);
}
void loop()
{
  CKC.run();
  if (CKC.CkC_Connected())
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
  static unsigned long sendTimer = 0;
  if (millis() - sendTimer > 1000)
  {
    sendTimer = millis();
    float temp = random(250, 350) / 10.0;
    float humidity = random(70, 80);
    MQTT.sendData("NHIET_DO", String(temp));
    MQTT.sendData("DO_AM", String(humidity));
    mqttClient.subscribe("V1");
    mqttClient.subscribe("V2");
    mqttClient.subscribe("V3");
  }
}
