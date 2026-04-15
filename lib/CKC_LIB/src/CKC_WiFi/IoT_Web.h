#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
String htmlPage()
{
  return R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
  <meta charset="UTF-8">
  <title>WiFi Config</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <style>
    * {
      box-sizing: border-box;
      font-family: 'Segoe UI', sans-serif;
    }

    body {
      margin: 0;
      background: #0b1220;
      color: #e5e7eb;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .card {
      background: #111827;
      padding: 30px;
      border-radius: 16px;
      width: 320px;
      box-shadow: 0 10px 25px rgba(0,0,0,0.4);
    }

    h2 {
      margin-bottom: 20px;
      text-align: center;
      color: #22c55e;
    }

    label {
      font-size: 14px;
      color: #9ca3af;
    }

    input {
      width: 100%;
      padding: 10px;
      margin-top: 5px;
      margin-bottom: 15px;
      border: none;
      border-radius: 8px;
      background: #1f2937;
      color: white;
    }

    input:focus {
      outline: 2px solid #22c55e;
    }

    button {
      width: 100%;
      padding: 12px;
      background: #22c55e;
      border: none;
      border-radius: 10px;
      color: #022c22;
      font-weight: bold;
      cursor: pointer;
      transition: 0.2s;
    }

    button:hover {
      background: #16a34a;
    }

    .footer {
      margin-top: 15px;
      text-align: center;
      font-size: 12px;
      color: #6b7280;
    }
  </style>
</head>

<body>

  <div class="card">
    <h2>WiFi Setup</h2>

    <form action="/save">
      <label>SSID</label>
      <input name="ssid" placeholder="Nhập tên WiFi">

      <label>Password</label>
      <input type="password" name="pass" placeholder="Nhập mật khẩu">

      <button type="submit">Lưu cấu hình</button>
    </form>

    <div class="footer">CKC IoT</div>
  </div>

</body>
</html>
  )rawliteral";
}