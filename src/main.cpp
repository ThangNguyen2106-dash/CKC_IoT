#include <Arduino.h>
#define CKC_DEBUG
#define BUTTON_MODE

const char *SSID = "CKC";
const char *PASS = "CKC2026";
#include <CKC.h>
void setup()
{
  Serial.begin(115200);
  CKC.begin(SSID, PASS);
}
void loop()
{
  CKC.run();
}


// #include "nvs_flash.h"
// #include <Arduino.h>
// void setup() {
//   Serial.begin(115200);

//   nvs_flash_erase(); // xóa toàn bộ flash NVS
//   nvs_flash_init();  // khởi tạo lại

//   Serial.println("Da xoa toan bo flash!");
// }

// void loop() {
// }