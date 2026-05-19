//========== // ==========//
#define NTC_pin 36
#define VCC_Pin 13
#define LED 2

#define SDA 32
#define SCL 33

#define RX 16
#define TX 17
//========== // ==========//

// EXAMPLE
// #include <Arduino.h>
// #include <khaibao_A.h>

// #define CKC_DEBUG
// #define BUTTON_MODE

// #include <Wire.h>
// #include <U8g2lib.h>
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// unsigned long time_P = 0;
// #include <CKC.h>
// const char *SSID = "";
// const char *PASS = "";

// const char *USERNAME = "";
// const char *USERPASS = "";

// // // #define USERNAME "0309231068@caothang.edu.vn"
// // // #define USERPASS 0aUJA3s2YIG5TCHKIaG5

// #define NTC_pin 36
// const unsigned long SampleTime = 10;
// unsigned long timer = 0;
// int Sum_adc = 0;
// int count = 0;

// float SensorNTC()
// {
//     static float lastTemp = 0.0;
//     // thông số mạch
//     float VCC = 3.3;
//     float ADC_MAX = 4095.0;
//     float R_Fixed = 9680.0;
//     // thông số NTC
//     float Beta = 3950.0;
//     float RBase = 10000.0;
//     float TBase = (273.15 + 25.0); // Nhiệt độ Keven khi ở 25°C
//     float RNTC = 0.0;
//     if (millis() - timer > SampleTime)
//     {
//         timer = millis();
//         count++;
//         int adc = analogRead(NTC_pin);
//         Sum_adc += adc;
//         if (count >= 100)
//         {
//             float adcAVG = Sum_adc / 100.0; // tính trung bình cộng ADC
//             float Vadc = ((adcAVG * VCC) / ADC_MAX);
//             count = 0;
//             Sum_adc = 0;
//             adcAVG = 0;
//             if ((Vadc < 0.001) || (Vadc > 3.29))
//             {
//                 Serial.println("Out of Range");
//                 count = 0;
//                 Sum_adc = 0;
//                 adcAVG = 0;
//                 return 0; // lỗi do ngõ vào ADC bị nhiễu nặng hoặc mất tính hiệu
//             }
//             // Tính toán điện trở và nhiệt độ
//             RNTC = R_Fixed * Vadc / (VCC - Vadc);
//             RNTC = RNTC * 1.25; // scale giá trị để gần đúng với thực tế
//             float tempK = 1.0 / (1.0 / TBase + (1.0 / Beta) * log(RNTC / RBase));
//             float tempC = tempK - 273.15;
//             lastTemp = tempC;
//         }
//     }
//     return lastTemp;
// }

// void hienthi(float Temp)
// {
//     u8g2.clearBuffer();

//     CKC_WiFI_TASK state = WiFi_TASK;

//     // =========================
//     // MODE CONNECTED
//     // =========================
//     if (state == MODE_CONNECTED)
//     {
//         if (serverMQTT._connect())
//         {
//             // ===== NHIỆT ĐỘ =====
//             u8g2.setFont(u8g2_font_logisoso24_tr);

//             char tempStr[16];
//             sprintf(tempStr, "%.2f", Temp);

//             u8g2.drawStr(0, 28, tempStr);

//             // độ C nhỏ góc phải
//             u8g2.setFont(u8g2_font_6x12_tr);
//             u8g2.drawUTF8(85, 10, "o");
//             u8g2.setFont(u8g2_font_logisoso24_tr);
//             u8g2.drawStr(90, 28, "C");

//             // ===== INFO =====
//             u8g2.setFont(u8g2_font_5x8_tr);

//             // line
//             u8g2.drawHLine(0, 32, 128);

//             // MODE
//             u8g2.drawStr(0, 41, "MODE : ONLINE");

//             // SSID
//             String ssid = WiFi.SSID();
//             String wifiLine = "SSID : " + ssid;
//             u8g2.drawStr(0, 50, wifiLine.c_str());

//             // MAC
//             String mac = WiFi.macAddress();
//             String macLine = "MAC  : " + mac;
//             u8g2.drawStr(0, 59, macLine.c_str());
//         }
//         if (!serverMQTT._connect())
//         {
//             u8g2.setFont(u8g2_font_6x12_tr);
//             u8g2.drawStr(20, 20, "Connecting...");
//             // ===== INFO =====
//             u8g2.setFont(u8g2_font_5x8_tr);

//             // line
//             u8g2.drawHLine(0, 32, 128);

//             // MODE
//             u8g2.drawStr(0, 41, "MODE : ONLINE");

//             // SSID
//             String ssid = WiFi.SSID();
//             String wifiLine = "SSID : " + ssid;
//             u8g2.drawStr(0, 50, wifiLine.c_str());

//             // MAC
//             String mac = WiFi.macAddress();
//             String macLine = "MAC  : " + mac;
//             u8g2.drawStr(0, 59, macLine.c_str());
//         }
//     }

//     // =========================
//     // AP CONFIG MODE
//     // =========================
//     else if (state == MODE_AP || state == RUN_AP_WEB)
//     {
//         // ===== TITLE =====
//         u8g2.setFont(u8g2_font_logisoso20_tr);
//         u8g2.drawStr(0, 24, "CONFIG");

//         // line
//         u8g2.drawHLine(0, 30, 128);

//         u8g2.setFont(u8g2_font_5x8_tr);

//         // AP NAME
//         String ap = WiFi.softAPSSID();
//         String apLine = "AP : " + ap;
//         u8g2.drawStr(0, 40, apLine.c_str());

//         // PASSWORD
//         u8g2.drawStr(0, 50, "PASS : CKC@2026");

//         // IP
//         IPAddress ip = WiFi.softAPIP();
//         String ipLine = "IP : " + ip.toString();
//         u8g2.drawStr(0, 60, ipLine.c_str());
//     }

//     // =========================
//     // CONNECTING
//     // =========================
//     else if (state == MODE_STA)
//     {
//         u8g2.setFont(u8g2_font_6x12_tr);
//         u8g2.drawStr(20, 50, "Connecting...");
//     }

//     // =========================
//     // RECONNECT
//     // =========================
//     else
//     {
//         u8g2.setFont(u8g2_font_6x12_tr);

//         if (WiFi.status() != WL_CONNECTED)
//         {
//             u8g2.drawStr(0, 45, "WiFi Lost !");
//         }
//         else
//         {
//             u8g2.drawStr(0, 45, "MQTT Reconnecting...");
//         }

//         String ssid = WiFi.SSID();
//         String line = "SSID : " + ssid;

//         u8g2.drawStr(0, 58, line.c_str());
//     }

//     u8g2.sendBuffer();
// }

// void timeEvent()
// {
//     float Temperature = SensorNTC();
//     CKC.writeTelemetry("Temp", Temperature);
// }

// void setup()
// {
//     Serial.begin(115200);
//     pinMode(NTC_pin, INPUT);
//     pinMode(VCC_Pin, OUTPUT);
//     pinMode(LED, OUTPUT);
//     digitalWrite(LED, LOW);
//     digitalWrite(VCC_Pin, HIGH);

//     Wire.begin(SDA, SCL);
//     u8g2.begin();
//     u8g2.clearBuffer();
//     u8g2.setFont(u8g2_font_6x12_tr);
//     u8g2.drawStr(20, 48, "RUNNING SYSTEM");
//     u8g2.sendBuffer();
//     delay(2000);

//     CKC.begin(SSID, PASS, USERNAME, USERPASS);
//     CKC.setTelemetry("Temp", NULL);
//     CKC.addTimeEvent(5000L, timeEvent);
// }
// void loop()
// {
//     float Temperature = SensorNTC();
//     hienthi(Temperature);
//     CKC.run();
// }