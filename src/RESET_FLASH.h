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
