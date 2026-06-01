#include <Arduino.h>
#define CKC_DEBUG
#define BUTTON_MODE

unsigned long time_P = 0;

#include <CKC.h>
const char *SSID = "";
const char *PASS = "";

const char *USERNAME = "";
const char *USERPASS = "";

#define RX 16 // chân RX
#define TX 17 // chân TX
#define DE 2  // chân chiều cho Module

float Humidity, Temperature, EC;
void read_Sensor1() // hàm đọc cảm biến Cảm biến nhiệt độ độ ẩm độ dẫn điện (EC) đất 3 trong 1 ES-SM-THEC-01
{
    uint16_t DATA1[3]; // ví dụ cần đọc 3 giá trị của cảm biến có thanh ghi liên tiếp nhau
    int RS = CKCModbus.readHoldingRegisterValue(
        2,      // Địa chỉ Slave của cảm biến
        0x0000, // Địa chỉ thanh ghi bắt đầu
        3,      // Số lượng thanh ghi cần đọc
        DATA1   // Dữ liệu trả về
    );

    if (RS > 0)
    {
        Humidity = DATA1[0] / 10.0;    // Xử lý giá trị độ ẩm
        Temperature = DATA1[1] / 10.0; // Xử lý giá trị nhiệt độ
        EC = DATA1[2];                 // Xử lý giá trị độ dẫn điện của đất
    }
    else
    {
        Serial.print("MODBUS ERROR: ");
        Serial.println(RS);
    }
}

float Voltage, Frequency, APower, Current;
void read_Sensor2() // hàm đọc cảm biến Đồng hồ đo vạn năng EM2M
{
    uint16_t DATA2[14]; // Đọc 2 thanh ghi của cảm biến EM2M
    int RS1 = CKCModbus.readHoldingRegisterValue(
        3,      // Địa chỉ Slave của cảm biến
        0x0E,   // Địa chỉ thanh ghi bắt đầu
        14,     // số lượng thanh ghi cần đọc
        DATA2); // Dữ liệu trả về
    if (RS1 > 0)
    {
        APower = CKCModbus.RegToFloat(DATA2[0], DATA2[1]);
        Voltage = CKCModbus.RegToFloat(DATA2[6], DATA2[7]);
        Current = CKCModbus.RegToFloat(DATA2[8], DATA2[9]);
        Frequency = CKCModbus.RegToFloat(DATA2[12], DATA2[13]);
    }
    else
    {
        Serial.print("MODBUS ERROR: ");
        Serial.println(RS1);
    }
}

void timeEvent()
{
    read_Sensor1();
    read_Sensor2();

    CKC.writeTelemetry("HUM", Humidity);
    CKC.writeTelemetry("TEMP", Temperature);
    CKC.writeTelemetry("EC", EC);

    CKC.writeTelemetry("POWER", APower);
    CKC.writeTelemetry("VOLT", Voltage);
    CKC.writeTelemetry("CURRENT", Current);
    CKC.writeTelemetry("FRE", Frequency);

    Serial.println("Độ ẩm: " + String(Humidity));
    Serial.println("Nhiệt độ: " + String(Temperature));
    Serial.println("Độ dẫn điện: " + String(EC));

    Serial.println("Công suất: " + String(APower));
    Serial.println("Điện áp: " + String(Voltage));
    Serial.println("Dòng điện: " + String(Current));
    Serial.println("Tần số: " + String(Frequency));
}

void setup()
{
    Serial.begin(115200);
    CKC.begin(SSID, PASS, USERNAME, USERPASS);
    CKCModbus.beginModbus(Serial1, 9600, RX, TX);
    CKCModbus.setTimeout(5000);

    CKC.setTelemetry("HUM", "TEMP", "EC", "POWER", "VOLT", "CURRENT", "FRE", NULL); // Khai báo Key dữ liệu muốn gửi đến máy chủ
    CKC.addTimeEvent(5000L, timeEvent);                                             // Khai báo chu kỳ thực hiện hàm gửi
}

void loop()
{
    CKC.run();
}