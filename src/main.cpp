#include <Arduino.h>
#include <khaibao_B.h>

// #define USERNAME "0309231068@caothang.edu.vn"
// #define USERPASS 0aUJA3s2YIG5TCHKIaG5

#define CKC_DEBUG
#define BUTTON_MODE

#include <Wire.h>
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE);

unsigned long time_P = 0;
#include <CKC.h>
const char *SSID = "Homi";
const char *PASS = "12345689";

void hienthi()
{
  u8g2.clearBuffer();

  CKC_WiFI_TASK state = WiFi_TASK;

  // =========================
  // MODE CONNECTED
  // =========================
  if (state == MODE_CONNECTED)
  {
    if (serverMQTT._connect())
    {
      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(20, 20, "RUNNING...");

      // ===== INFO =====
      u8g2.setFont(u8g2_font_5x8_tr);

      // line
      u8g2.drawHLine(0, 32, 128);

      // MODE
      u8g2.drawStr(0, 41, "MODE : ONLINE");

      // SSID
      String ssid = WiFi.SSID();
      String wifiLine = "SSID : " + ssid;
      u8g2.drawStr(0, 50, wifiLine.c_str());

      // MAC
      String mac = WiFi.macAddress();
      String macLine = "MAC  : " + mac;
      u8g2.drawStr(0, 59, macLine.c_str());
    }
    if (!serverMQTT._connect())
    {
      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(20, 20, "Connecting...");
      // ===== INFO =====
      u8g2.setFont(u8g2_font_5x8_tr);

      // line
      u8g2.drawHLine(0, 32, 128);

      // MODE
      u8g2.drawStr(0, 41, "MODE : ONLINE");

      // SSID
      String ssid = WiFi.SSID();
      String wifiLine = "SSID : " + ssid;
      u8g2.drawStr(0, 50, wifiLine.c_str());

      // MAC
      String mac = WiFi.macAddress();
      String macLine = "MAC  : " + mac;
      u8g2.drawStr(0, 59, macLine.c_str());
    }
  }

  // =========================
  // AP CONFIG MODE
  // =========================
  else if (state == MODE_AP || state == RUN_AP_WEB)
  {
    // ===== TITLE =====
    u8g2.setFont(u8g2_font_logisoso20_tr);
    u8g2.drawStr(0, 24, "CONFIG");

    // line
    u8g2.drawHLine(0, 30, 128);

    u8g2.setFont(u8g2_font_5x8_tr);

    // AP NAME
    String ap = WiFi.softAPSSID();
    String apLine = "AP : " + ap;
    u8g2.drawStr(0, 40, apLine.c_str());

    // PASSWORD
    u8g2.drawStr(0, 50, "PASS : CKC@2026");

    // IP
    IPAddress ip = WiFi.softAPIP();
    String ipLine = "IP : " + ip.toString();
    u8g2.drawStr(0, 60, ipLine.c_str());
  }

  // =========================
  // CONNECTING
  // =========================
  else if (state == MODE_STA)
  {
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(20, 50, "Connecting...");
  }

  // =========================
  // RECONNECT
  // =========================
  else
  {
    u8g2.setFont(u8g2_font_6x12_tr);

    if (WiFi.status() != WL_CONNECTED)
    {
      u8g2.drawStr(0, 45, "WiFi Lost !");
    }
    else
    {
      u8g2.drawStr(0, 45, "MQTT Reconnecting...");
    }

    String ssid = WiFi.SSID();
    String line = "SSID : " + ssid;

    u8g2.drawStr(0, 58, line.c_str());
  }

  u8g2.sendBuffer();
}

void timeEvent()
{
  // CKC.writeTelemetry("SS", SENSOR);
}

void setup()
{
  Serial.begin(115200);

  Wire.begin(SDA, SCL);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(20, 48, "RUNNING SYSTEM");
  u8g2.sendBuffer();
  delay(2000);

  CKC.begin(SSID, PASS);
  CKC.setTelemetry("SS", NULL);
  CKC.addTimeEvent(5000L, timeEvent);
}
void loop()
{
  hienthi();
  CKC.run();
}