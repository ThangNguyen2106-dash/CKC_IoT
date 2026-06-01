// #include <Arduino.h>
// #define CKC_DEBUG
// #define BUTTON_MODE

// unsigned long time_P = 0;

// #include <CKC.h>
// const char *SSID = "";
// const char *PASS = "";

// const char *USERNAME = "";
// const char *USERPASS = "";

// #define RX 16 // chân RX
// #define TX 17 // chân TX
// #define DE 2  // chân chiều cho Module

// float Humidity, Temperature, EC;
// void read_Sensor() // hàm đọc cảm biến Cảm biến nhiệt độ độ ẩm độ dẫn điện (EC) đất 3 trong 1 ES-SM-THEC-01
// {
//     uint16_t DATA[3]; // ví dụ cần đọc 3 giá trị của cảm biến có thanh ghi liên tiếp nhau
//     int RS = CKCModbus.readHoldingRegisterValue(
//         2,      // Địa chỉ Slave của cảm biến
//         0x0000, // Địa chỉ thanh ghi bắt đầu
//         3,      // Số lượng thanh ghi cần đọc
//         DATA    // Dữ liệu trả về
//     );

//     if (RS > 0)
//     {
//         Humidity = DATA[0] / 10.0;    // Xử lý giá trị độ ẩm
//         Temperature = DATA[1] / 10.0; // Xử lý giá trị nhiệt độ
//         EC = DATA[2];                 // Xử lý giá trị độ dẫn điện của đất
//     }
//     else
//     {
//         Serial.print("MODBUS ERROR: ");
//         Serial.println(RS);
//     }
// }

// void timeEvent()
// {
//     read_Sensor();
//     CKC.writeTelemetry("HUM", Humidity);
//     CKC.writeTelemetry("TEMP", Temperature);
//     CKC.writeTelemetry("EC", EC);
// }

// void setup()
// {
//     Serial.begin(115200);
//     CKC.begin(SSID, PASS, USERNAME, USERPASS);
//     CKCModbus.beginModbus(Serial1, 9600, RX, TX, SERIAL_8N1);
//     CKCModbus.setTimeout(1000);

//     CKC.setTelemetry("HUM", "TEMP", "EC", NULL); // Khai báo Key dữ liệu muốn gửi đến máy chủ
//     CKC.addTimeEvent(5000L, timeEvent);          // Khai báo chu kỳ thực hiện hàm gửi
// }

// void loop()
// {
//     CKC.run();
// }