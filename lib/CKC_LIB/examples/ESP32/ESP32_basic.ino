
/*************************************************************
  Download latest CKC_lib here:
      https://github.com/PLGdevo/CKC_LIB.git

    CKC Website:                https://ait.caothang.edu.vn/login
    Documentation:              https://your-docs.com
    Community/Forum:            https://your-forum.com
    Follow us:                  https://www.fb.com/your-page

 *************************************************************/

/*
 * CKC_lib - IoT Communication & Control Library
 *
 * Copyright (c) 2026 CKC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies.
 *
 * Conditions:
 *  - The above copyright notice must be included in all copies.
 *  - Do not misrepresent the origin of this software.
 *
 * Disclaimer:
 *  This software is provided "as is", without warranty of any kind.
 *  CKC shall not be liable for any damages arising from its use.
 */

// Bật debug log ra Serial Monitor
#define CKC_DEBUG

// Bật chế độ nút nhấn (tuỳ thư viện bạn xử lý)
#define BUTTON_MODE

// Thông tin WiFi
const char *SSID = "Nhập tên WiFi";
const char *PASS = "Nhập mật khẩu WiFi";

// Thông tin đăng nhập từ WebServer
const char *USER = "Nhập Email từ hồ sơ";
const char *PASS = "Nhập mật khẩu từ hồ sơ";

//=======================================================================//
// Ngoài ra có thể thay đổi thông tin WiFi và thông tin đăng nhập từ Web //
//====================================================================== //

// Include thư viện chính
#include <CKC.h>

/*
==========================================================
  SETUP
==========================================================
*/

// Hàm thực thi gửi dữ liệu theo chu kì cài đặt
void timeEvent()
{
  float nhietdo = 30.0;
  CKC.writeTelemetry("Temp", nhietdo);
}

void setup()
{
  Serial.begin(115200); // Khởi tạo Serial
  pinMode(26, OUTPUT);  // LED báo trạng thái (CÓ THỂ KHÔNG CẦN)

  // Kết nối WiFi + Server CKC
  CKC.begin(SSID, PASS);

  // Khai báo các key telemetry sẽ gửi lên server
  CKC.setTelemetry("TEM", "HUM", NULL);

  // Khai báo chu kì thực thi gửi dữ liệu
  CKC.addTimeEvent(5000L, timeEvent);
}

/*
==========================================================
  LOOP
==========================================================
*/
void loop()
{
  // Hàm chạy chính của thư viện (bắt buộc)
  CKC.run();
}