// #include <Arduino.h>
// #define CKC_DEBUG
// #define BUTTON_MODE
// #include <CKC.h>
// #include <PZEM004Tv30.h>

// const char *SSID = "Thắng Nguyễn";
// const char *PASS = "mt21062005";

// const char *USERNAME = "0309231068@caothang.edu.vn";
// const char *USERPASS = "4dfAudwntoocVw6rJU7B";

// #define RX_ 16 // chân RX cho Module Modbus
// #define TX_ 17 // chân TX cho Module Modbus

// #define rx 18 // Chân RX cho Module PZEM-004T
// #define tx 19 // Chân TX cho Module PZEM-004T

// // Đọc cảm biến SHT40
// float Humidity, Temperature;
// void read_Sensor1()
// {
//     uint16_t DATA[2];
//     int RS1 = CKCModbus.readHoldingRegisterValue(1, 0x0000, 2, DATA);
//     if (RS1 > 0)
//     {
//         Humidity = DATA[0] / 10.0;
//         Temperature = DATA[1] / 10.0;
//     }
//     else
//     {
//         Humidity = NAN;
//         Temperature = NAN;
//         Serial.println("MODBUS FAIL");
//         Serial.print(RS1);
//     }
// }

// // Đọc cảm biến năng lượng
// PZEM004Tv30 pzem(Serial1, rx, tx);
// float voltage, current, power, energy;
// void read_sensor2()
// {
//     voltage = pzem.voltage();
//     current = pzem.current();
//     power = pzem.power();
//     energy = pzem.energy();
//     if (isnan(voltage))
//     {
//         voltage = current = power = energy = NAN;
//         Serial.println("FAIL / NO DATA");
//     }
//     else
//     {
//         Serial.print("V: ");
//         Serial.print(voltage);
//         Serial.print(" V | ");
//         Serial.print("I: ");
//         Serial.print(current);
//         Serial.print(" A | ");
//         Serial.print("P: ");
//         Serial.print(power);
//         Serial.print(" W | ");
//         Serial.print("E: ");
//         Serial.print(energy);
//         Serial.println(" kWh");
//     }
// }

// // Đọc cảm biến áp suất
// #define ADC_MAX 4095.0
// #define P_MIN 0.0
// #define P_MAX -101.0
// float V_REF = 3.3;
// //=========================
// void sortArray(uint16_t arr[], int n)
// {
//     for (int i = 0; i < n - 1; i++)
//     {
//         for (int j = i + 1; j < n; j++)
//         {
//             if (arr[i] > arr[j])
//             {
//                 uint16_t t = arr[i];
//                 arr[i] = arr[j];
//                 arr[j] = t;
//             }
//         }
//     }
// }

// #define PRESSURE_PIN2 36
// float pressure2 = 0;
// unsigned long timer2 = 0;
// const uint32_t SampleTime2 = 10;
// const int SAMPLE_COUNT2 = 51;
// uint16_t adcBuffer2[SAMPLE_COUNT2];
// uint8_t count2 = 0;
// void read_sensor3()
// {
//     if (millis() - timer2 < SampleTime2)
//         return;
//     timer2 = millis();
//     adcBuffer2[count2] = analogReadMilliVolts(PRESSURE_PIN2);
//     count2++;
//     if (count2 < SAMPLE_COUNT2)
//         return;
//     sortArray(adcBuffer2, SAMPLE_COUNT2);
//     float V_ADC = adcBuffer2[SAMPLE_COUNT2 / 2] / 1000.0;
//     float Vsensor = V_ADC * 1.51;
//     pressure2 = (Vsensor * (-24.8775098)) + 23.3872549;
//     Serial.println("VADC: " + String(V_ADC, 3));
//     Serial.println("VSENSOR: " + String(Vsensor, 3));
//     Serial.println("P: " + String(pressure2, 2));
//     count2 = 0;
// }

// // Đọc cảm biến Áp suất xuất ra 4-20mA
// #define PRESSURE_PIN 39
// float pressure1 = 0;
// unsigned long timer1 = 0;
// const uint32_t SampleTime1 = 10;
// const int SAMPLE_COUNT1 = 51;
// uint16_t adcBuffer1[SAMPLE_COUNT1];
// uint8_t count1 = 0;
// void read_sensor4()
// {
//     if (millis() - timer1 < SampleTime1)
//         return;
//     timer1 = millis();
//     adcBuffer1[count1] = analogRead(PRESSURE_PIN);
//     count1++;
//     if (count1 < SAMPLE_COUNT1)
//         return;
//     sortArray(adcBuffer1, SAMPLE_COUNT1);
//     float ADC_MED = adcBuffer1[SAMPLE_COUNT1 / 2];
//     float V_ADC = (ADC_MED * V_REF) / ADC_MAX;
//     pressure1 = (V_ADC / V_REF) * (P_MAX - P_MIN) + P_MIN;
//     Serial.print("Cam bien ap suat 1: ");
//     Serial.print(pressure1);
//     Serial.println(" kPa");
//     count1 = 0;
// }

// void timeSensor()
// {
//     // CKC.writeTelemetry("HUM", Humidity);
//     // CKC.writeTelemetry("TEMP", Temperature);

//     // CKC.writeTelemetry("VOLTAGE", voltage);
//     // CKC.writeTelemetry("CURRENT", current);
//     // CKC.writeTelemetry("POWER", power);
//     // CKC.writeTelemetry("ENERGY", energy);

//     // CKC.writeTelemetry("PRESSURE", pressure1);
//     CKC.writeTelemetry("PRESSURE2", pressure2);
// }

// void setup()
// {
//     pinMode(PRESSURE_PIN, INPUT);
//     Serial.begin(115200);
//     CKCModbus.beginModbus(Serial2, 4800, RX_, TX_, SERIAL_8N1);
//     CKCModbus.setTimeout(1000);
//     Serial1.begin(9600, SERIAL_8N1, rx, tx);

//     CKC.begin(SSID, PASS, USERNAME, USERPASS);
//     CKC.setTelemetry("HUM", "TEMP");                           // Khai báo Key dữ liệu về nhiệt độ độ ẩm gửi lên máy chủ
//     CKC.setTelemetry("VOLTAGE", "CURRENT", "POWER", "ENERGY"); // khai báo Key dữ liệu về điện năng gửi lên máy chủ
//     CKC.setTelemetry("PRESSURE");
//     CKC.addTimeEvent(5000L, timeSensor); // Khai báo chu kỳ thực hiện hàm gửi
// }

// void loop()
// {
//     CKC.run();
//     // read_Sensor1();
//     // read_sensor2();
//     read_sensor3();
//     // read_sensor4();
// }

#include <Arduino.h>
#include "nvs_flash.h"
void setup()
{
    Serial.begin(115200);
    nvs_flash_erase(); // xóa toàn bộ flash NVS
    nvs_flash_init();  // khởi tạo lại
    Serial.println("Da xoa toan bo flash!");
}
void loop()
{
}
