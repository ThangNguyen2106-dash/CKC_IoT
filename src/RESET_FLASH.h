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


// #include <Arduino.h>
// #define CKC_DEBUG
// #define BUTTON_MODE

// #include <CKC.h>
// #include <PZEM004Tv30.h>

// const char *SSID = "Thắng Nguyễn";
// const char *PASS = "mt21062005";

// const char *USERNAME = "0309231068@caothang.edu.vn";
// const char *USERPASS = "4dfAudwntoocVw6rJU7B";

// #define RX_ 16  // chân RX cho Module Modbus
// #define TX_ 17  // chân TX cho Module Modbus

// #define rx 18  // Chân RX cho Module PZEM-004T
// #define tx 19  // Chân TX cho Module PZEM-004T

// #define PRESSURE_PIN 36
// #define ADC_MAX 4095.0
// #define ADC_REF 3.3
// #define P_MIN -101.0
// #define P_MAX 0.0
// float readPressure() {
//   int ADC = analogRead(PRESSURE_PIN);
//   float pressure = (ADC / ADC_MAX);
// }

// float Humidity, Temperature;
// void read_Sensor1()  // hàm đọc cảm biến Cảm biến nhiệt độ độ ẩm độ SHT40-58H-RS485
// {
//   uint16_t DATA[2];
//   int RS1 = CKCModbus.readHoldingRegisterValue(
//     1,       // Địa chỉ Slave của cảm biến
//     0x0000,  // Địa chỉ thanh ghi bắt đầu đọc
//     2,       // Số lượng thanh ghi cần đọc
//     DATA);   // Dữ liệu được trả về
//   if (RS1 > 0) {
//     Humidity = DATA[0] / 10.0;
//     Temperature = DATA[1] / 10.0;
//     Serial.println("Humidity: " + String(Humidity));
//     Serial.println("Temperature: " + String(Temperature));
//   } else {
//     Serial.print("MODBUS ERROR: ");
//     Serial.println(RS1);
//   }
// }

// PZEM004Tv30 pzem(Serial1, rx, tx);
// float voltage, current, power, energy;
// void read_sensor2() {
//   voltage = pzem.voltage();
//   current = pzem.current();
//   power = pzem.power();
//   energy = pzem.energy();
//   if (isnan(voltage)) {
//     Serial.println("FAIL / NO DATA");
//   } else {
//     Serial.print("V: ");
//     Serial.print(voltage);
//     Serial.print(" V | ");
//     Serial.print("I: ");
//     Serial.print(current);
//     Serial.print(" A | ");
//     Serial.print("P: ");
//     Serial.print(power);
//     Serial.print(" W | ");
//     Serial.print("E: ");
//     Serial.print(energy);
//     Serial.println(" kWh");
//   }
// }

// #define PRESSURE_PIN 36
// #define ADC_MAX 4095.0
// #define V_REF 3.3
// #define P_MIN -101.0
// #define P_MAX 0.0
// float pressure = 0;
// unsigned long timer3 = 0;
// const unsigned long SampleTime3 = 10;
// int count = 0;
// uint32_t sum_ADC = 0;
// void read_sensor3() {
//   static float lastPressure = 0;
//   if (millis() - timer3 < SampleTime3)
//     return;
//   timer3 = millis();
//   int ADC = analogRead(PRESSURE_PIN);
//   sum_ADC += ADC;
//   count++;
//   if (count >= 100) {
//     float ADC_AVG = sum_ADC / 100.0;
//     float V_ADC = (ADC_AVG * V_REF) / ADC_MAX;
//     count = 0;
//     sum_ADC = 0;
//     if ((V_ADC < 0.001) || (V_ADC > 3.29)) {
//       Serial.println("Out of Range");
//       return;
//     }
//     pressure = (V_ADC / V_REF) * (P_MAX - P_MIN) + P_MIN;
//     if (abs(pressure - lastPressure) > 0.2) {
//       lastPressure = pressure;
//       Serial.print("Pressure: ");
//       Serial.print(pressure);
//       Serial.println(" kPa");
//     }
//   }
// }


// void timeEvent() {
//   // read_Sensor1();
//   // read_sensor2();
//   read_sensor3();
// }

// void setup() {
//   Serial.begin(115200);
//   CKC.begin(SSID, PASS, USERNAME, USERPASS);

//   CKC.setTelemetry("HUM", "TEMP", "EC");  // Khai báo Key dữ liệu muốn gửi đến máy chủ
//   CKC.addTimeEvent(5000L, timeEvent);     // Khai báo chu kỳ thực hiện hàm gửi

//   CKCModbus.beginModbus(Serial2, 4800, RX_, TX_, SERIAL_8N1);
//   CKCModbus.setTimeout(1000);

//   Serial1.begin(9600, SERIAL_8N1, rx, tx);
// }

// void loop() {
//   CKC.run();
// }