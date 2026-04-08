// // #include <Web.h>
// // WebServer server(80);
// // Preferences prefs;

// // // ===== CONFIG ===== //
// // #define TIME_CHECK 10000
// // #define WIFI_TIMEOUT 10000
// // #define WiFi_MAX 5
// // String ssid_list[WiFi_MAX];
// // String pass_list[WiFi_MAX];

// // unsigned long lastCheck = 0;
// // bool apMode = false;

// // // WiFiAP
// // const char *ssidAP = "ESP32_IoT_CKC";
// // const char *passAP = "12345678";

// // // ===== SAVE WIFI ===== //
// // void SaveWiFi(String newSSID, String newPASS)
// // {
// //   prefs.begin("wifi", false);
// //   for (int i = WiFi_MAX - 1; i > 0; i--)
// //   {
// //     prefs.putString(("ssid" + String(i)).c_str(), ssid_list[i - 1]);
// //     prefs.putString(("pass" + String(i)).c_str(), pass_list[i - 1]);
// //   }
// //   prefs.putString("ssid0", newSSID);
// //   prefs.putString("pass0", newPASS);
// //   prefs.end();
// // }
// // void handleSave()
// // {
// //   String newSSID = server.arg("ssid");
// //   String newPASS = server.arg("pass");
// //   SaveWiFi(newSSID, newPASS);
// //   server.send(200, "text/html", "Saved! Restarting...");
// //   delay(2000);
// //   ESP.restart();
// // }

// // // ===== LOAD WIFI ===== //
// // void loadWiFi()
// // {
// //   prefs.begin("wifi", true);
// //   for (int i = 0; i < WiFi_MAX; i++)
// //   {
// //     ssid_list[i] = prefs.getString(("ssid" + String(i)).c_str(), "");
// //     pass_list[i] = prefs.getString(("pass" + String(i)).c_str(), "");
// //   }
// //   prefs.end();
// // }

// // // ===== START AP ===== //
// // void startAP()
// // {
// //   apMode = true;
// //   WiFi.mode(WIFI_AP);
// //   WiFi.softAP(ssidAP, passAP);
// //   Serial.println("AP Mode Started");
// //   Serial.println(WiFi.softAPIP());
// //   server.on("/", []()
// //             { server.send(200, "text/html", htmlPage()); });
// //   server.on("/save", handleSave);
// //   server.begin();
// // }

// // // ===== CONNECT WIFI ===== //
// // void setupWiFi()
// // {
// //   loadWiFi();
// //   WiFi.mode(WIFI_STA);
// //   Serial.println("Scanning WiFi...");
// //   int n = WiFi.scanNetworks();
// //   if (n <= 0)
// //   {
// //     Serial.println("No WiFi found → Start AP");
// //     startAP();
// //     return;
// //   }
// //   for (int j = 0; j < WiFi_MAX; j++)
// //   {
// //     if (ssid_list[j] == "")
// //       continue;
// //     for (int i = 0; i < n; i++)
// //     {
// //       if (WiFi.SSID(i) == ssid_list[j])
// //       {
// //         Serial.println("Connecting to: " + ssid_list[j]);

// //         WiFi.begin(ssid_list[j].c_str(), pass_list[j].c_str());

// //         unsigned long start = millis();

// //         while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
// //         {
// //           delay(300);
// //           Serial.print(".");
// //         }
// //         if (WiFi.status() == WL_CONNECTED)
// //         {
// //           Serial.println("\nConnected!");
// //           Serial.println(WiFi.localIP());
// //           return;
// //         }
// //         else
// //         {
// //           Serial.println("\nFailed → thử WiFi khác");
// //         }
// //       }
// //     }
// //   }
// //   Serial.println("No known WiFi :(( Start AP");
// //   startAP();
// // }

// // void setup()
// // {
// //   Serial.begin(115200);
// //   delay(1000);
// //   Serial.println("\nBooting...");
// //   setupWiFi();
// //   if (!apMode)
// //   {
// //     Serial.println("Device running in STA mode");
// //     Serial.print("IP: ");
// //     Serial.println(WiFi.localIP());
// //   }
// // }
// // void loop()
// // {
// //   if (apMode)
// //   {
// //     server.handleClient();
// //   }
// // }

// // #include <WiFi.h>
// // // Hàm chuyển kiểu mã hóa sang text
// // const char *getEncryptionType(wifi_auth_mode_t type)
// // {
// //   switch (type)
// //   {
// //   case WIFI_AUTH_OPEN:
// //     return "OPEN";
// //   case WIFI_AUTH_WEP:
// //     return "WEP";
// //   case WIFI_AUTH_WPA_PSK:
// //     return "WPA_PSK";
// //   case WIFI_AUTH_WPA2_PSK:
// //     return "WPA2_PSK";
// //   case WIFI_AUTH_WPA_WPA2_PSK:
// //     return "WPA_WPA2_PSK";
// //   case WIFI_AUTH_WPA2_ENTERPRISE:
// //     return "WPA2_ENTERPRISE";
// //   default:
// //     return "UNKNOWN";
// //   }
// // }
// // void setup()
// // {
// //   Serial.begin(115200);
// //   delay(1000);

// //   Serial.println("\n--- WIFI SCAN START ---");

// //   // Set mode station (không phát AP)
// //   WiFi.mode(WIFI_STA);
// //   WiFi.disconnect();
// //   delay(100);

// //   // Scan WiFi
// //   int n = WiFi.scanNetworks();

// //   if (n == 0)
// //   {
// //     Serial.println("Không tìm thấy mạng WiFi nào!");
// //   }
// //   else
// //   {
// //     Serial.printf("Tìm thấy %d mạng:\n\n", n);

// //     for (int i = 0; i < n; ++i)
// //     {
// //       Serial.printf("%d. SSID: %s\n", i + 1, WiFi.SSID(i).c_str());
// //       Serial.printf("   RSSI: %d dBm\n", WiFi.RSSI(i));
// //       Serial.printf("   Channel: %d\n", WiFi.channel(i));
// //       Serial.printf("   Security: %s\n", getEncryptionType(WiFi.encryptionType(i)));
// //       Serial.println("---------------------------");
// //     }
// //   }

// //   Serial.println("--- WIFI SCAN END ---");
// // }

// // void loop()
// // {
// //   // không cần lặp
// // }

// #ifndef CKC_ESP32
// #define CKC_ESP32

// #include <Arduino.h>
// #include <WiFi.h>
// #include <WebServer.h>
// #include <Preferences.h>

// // ================= CONFIG =================
// #define WIFI_AP_IP IPAddress(192, 168, 4, 1)

// #define AP_WIFI_NAME "CKC:"
// #define AP_WIFI_PASS "12345678"

// #define FLASH_BTN 0

// // ================= ENUM =================
// enum CKC_WiFI_TASK
// {
//     MODE_STA,
//     MODE_AP,
//     MODE_CONNECTED
// };

// CKC_WiFI_TASK WiFi_TASK = MODE_STA;

// // ================= CLASS =================
// template <class Transport>
// class CKC_PnP
// {
// public:
//     void init();
//     void run();

// private:
//     // WiFi info
//     char _sta_ssid[32] = "";
//     char _sta_pass[32] = "";

//     char _ap_ssid[32];
//     char _ap_pass[32] = AP_WIFI_PASS;

//     // time
//     unsigned long t0, t1;

//     // button
//     unsigned long pressStart = 0;
//     bool triggered = false;

//     // web
//     WebServer _server = WebServer(80);
//     Preferences _prefs;
//     bool webStarted = false;

// private:
//     void connectSTA();
//     void startAP();
//     void handleAP();
//     void startWeb();
//     void loadWiFi();
//     void saveWiFi(String ssid, String pass);
//     void handleButton();
// };

// // ================= IMPLEMENT =================

// template <class T>
// void CKC_PnP<T>::init()
// {
//     Serial.begin(115200);

//     loadWiFi();

//     String mac = WiFi.macAddress();
//     mac.replace(":", "");

//     snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", AP_WIFI_NAME, mac.c_str());

//     pinMode(FLASH_BTN, INPUT_PULLUP);

//     t1 = millis();
// }

// // ===== LOAD WIFI =====
// template <class T>
// void CKC_PnP<T>::loadWiFi()
// {
//     _prefs.begin("wifi", true);
//     String ssid = _prefs.getString("ssid", "");
//     String pass = _prefs.getString("pass", "");
//     _prefs.end();

//     if (ssid.length())
//     {
//         strcpy(_sta_ssid, ssid.c_str());
//         strcpy(_sta_pass, pass.c_str());
//     }
// }

// // ===== SAVE WIFI =====
// template <class T>
// void CKC_PnP<T>::saveWiFi(String ssid, String pass)
// {
//     _prefs.begin("wifi", false);
//     _prefs.putString("ssid", ssid);
//     _prefs.putString("pass", pass);
//     _prefs.end();
// }

// // ===== CONNECT STA =====
// template <class T>
// void CKC_PnP<T>::connectSTA()
// {
//     Serial.println("Connecting WiFi...");

//     WiFi.mode(WIFI_STA);
//     WiFi.begin(_sta_ssid, _sta_pass);

//     unsigned long start = millis();

//     while (WiFi.status() != WL_CONNECTED && millis() - start < 15000)
//     {
//         Serial.print(".");
//         delay(500);
//         handleButton();
//     }

//     if (WiFi.status() == WL_CONNECTED)
//     {
//         Serial.println("\nConnected!");
//         Serial.println(WiFi.localIP());
//         WiFi_TASK = MODE_CONNECTED;
//     }
//     else
//     {
//         Serial.println("\nFailed -> AP mode");
//         WiFi_TASK = MODE_AP;
//     }
// }

// // ===== START AP =====
// template <class T>
// void CKC_PnP<T>::startAP()
// {
//     WiFi.mode(WIFI_AP);
//     WiFi.softAP(_ap_ssid, _ap_pass);
//     WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, IPAddress(255, 255, 255, 0));

//     Serial.println("AP Started");
//     Serial.println(_ap_ssid);
//     Serial.println(WIFI_AP_IP);

//     startWeb();
// }

// // ===== WEB HTML =====
// String htmlPage()
// {
//     return R"rawliteral(
//     <html>
//     <body>
//     <h2>WiFi Config</h2>
//     <form action="/save">
//     SSID:<br>
//     <input name="ssid"><br>
//     PASS:<br>
//     <input name="pass" type="password"><br><br>
//     <input type="submit" value="Save">
//     </form>
//     </body>
//     </html>
//     )rawliteral";
// }

// // ===== START WEB =====
// template <class T>
// void CKC_PnP<T>:: startWeb()
// {
//     _server.on("/", [this]()
//                { _server.send(200, "text/html", htmlPage()); });

//     _server.on("/save", [this]()
//                {
//         String ssid = _server.arg("ssid");
//         String pass = _server.arg("pass");

//         saveWiFi(ssid, pass);

//         _server.send(200, "text/html", "Saved! Restarting...");
//         delay(2000);

//         WiFi_TASK = MODE_STA;
//         t1 = millis(); });

//     _server.begin();
//     webStarted = true;
// }

// // ===== HANDLE AP =====
// template <class T>
// void CKC_PnP<T>::handleAP()
// {
//     if (!webStarted)
//         startAP();

//     _server.handleClient();
// }

// // ===== BUTTON HOLD 5s -> AP =====
// template <class T>
// void CKC_PnP<T>::handleButton()
// {
//     bool pressed = (digitalRead(FLASH_BTN) == LOW);

//     if (pressed)
//     {
//         if (pressStart == 0)
//             pressStart = millis();

//         if (!triggered && millis() - pressStart > 5000)
//         {
//             triggered = true;
//             Serial.println("Force AP mode");
//             WiFi_TASK = MODE_AP;
//             webStarted = false;
//         }
//     }
//     else
//     {
//         pressStart = 0;
//         triggered = false;
//     }
// }

// // ===== MAIN RUN =====
// template <class T>
// void CKC_PnP<T>::run()
// {
//     switch (WiFi_TASK)
//     {
//     case MODE_STA:
//         connectSTA();
//         break;

//     case MODE_AP:
//         handleAP();
//         break;

//     case MODE_CONNECTED:
//         if (WiFi.status() != WL_CONNECTED)
//         {
//             WiFi_TASK = MODE_STA;
//         }
//         break;
//     }

//     handleButton();
// }

// #endif


//========== INIT ==========//
// template <class Transport>
// inline void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass)
// {
//     server = new WebServer(80);
//     strcpy(_sta_ssid, sta_ssid);
//     strcpy(_sta_pass, sta_pass);
//     String MAC = WiFi.macAddress();
//     strcpy(_mac, MAC.c_str());
//     snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", _ap_ssid, _mac);

//     CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
//     CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
//     CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", _sta_ip);
//     CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);

//     delay(100);
//     t1 = millis();
//     pinMode(FLASH_BTN, INPUT_PULLUP); // nút kéo xuống GND khi nhấn
// }
// //========== STA CONNECTION ==========//
// template <class Transport>
// inline void CKC_PnP<Transport>::CKC_state_Connect_STA()
// {
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(_sta_ssid, _sta_pass);
//     while (WiFi.status() != WL_CONNECTED && millis() - t1 <= this->time_sta)
//     {
//         if (millis() - t0 > 1000)
//         {
//             CKC_LOG_DEBUG("WIFI", "CONNECTING ___ %ds\r", (millis() - t1) / 1000);
//             t0 = millis();
//         }
//         handler_button();
//     }
//     if (WiFi.status() == WL_CONNECTED)
//     {
//         CKC_LOG_DEBUG("WIFI", "WIFI_CONNECTED :)) ");
//         CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", WiFi.localIP().toString());
//         CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
//         serverMQTT.begin();
//         CKC_LOG_DEBUG("TAG", "\r\n"
//                              "  ____  _  __   ____   "
//                              "\r\n"
//                              " / ___|| |/ /  / ___|  "
//                              "\r\n"
//                              "| |    | ' /  | |      "
//                              "\r\n"
//                              "| |___ | . \\  | |___   "
//                              "\r\n"
//                              " \\____||_|\\_\\  \\____|  "
//                              "\r\n");
//         WiFi_TASK = MODE_CONNECTED;
//     }
//     else
//     {
//         CKC_LOG_DEBUG("WIFI", "WIFI_CONNECT_FALSE !!!!!! ");
//         CKC_LOG_DEBUG("WIFI", "RUN_AP");
//         CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
//         CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
//         CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
//         CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);
//         WiFi_TASK = MODE_AP;
//         WiFi.mode(WIFI_OFF);
//         WiFi.mode(WIFI_AP);
//         _ipAddr.fromString(_ap_ip);
//         WiFi.softAP(_ap_ssid, _ap_pass);
//         t2 = millis();
//     }
// }

// //========== AP CONNECTION ==========//
// template <class Transport>
// inline void CKC_PnP<Transport>::SaveWiFi(String newSSID, String newPASS)
// {
//     prefs.begin("wifi", false);
//     for (int i = WIFI_MAX - 1; i > 0; i--)
//     {
//         prefs.putString(("ssid" + String(i)).c_str(), ssid_list[i - 1]);
//         prefs.putString(("pass" + String(i)).c_str(), pass_list[i - 1]);
//     }
//     prefs.putString("ssid0", newSSID);
//     prefs.putString("pass0", newPASS);
//     prefs.end();
// }
// template <class Transport>
// inline void CKC_PnP<Transport>::handleSave()
// {
//     String newSSID = server->arg("ssid");
//     String newPASS = server->arg("pass");
//     SaveWiFi(newSSID, newPASS);
//     server->send(200, "text/html", "Saved! Restarting...");
//     delay(2000);
// }
// template <class Transport>
// inline void CKC_PnP<Transport>::loadWiFi()
// {
//     prefs.begin("wifi", true);
//     for (int i = 0; i < WIFI_MAX; i++)
//     {
//         ssid_list[i] = prefs.getString(("ssid" + String(i)).c_str(), "");
//         pass_list[i] = prefs.getString(("pass" + String(i)).c_str(), "");
//     }
//     prefs.end();
// }
// template <class Transport>
// inline bool CKC_PnP<Transport>::connectMultiWiFi()
// {
//     loadWiFi();
//     WiFi.mode(WIFI_STA);
//     int n = WiFi.scanNetworks();
//     if (n <= 0)
//         return false;
//     for (int j = 0; j < WIFI_MAX; j++)
//     {
//         if (ssid_list[j] == "")
//             continue;
//         for (int i = 0; i < n; i++)
//         {
//             if (WiFi.SSID(i) == ssid_list[j])
//             {
//                 WiFi.begin(ssid_list[j].c_str(), pass_list[j].c_str());

//                 unsigned long start = millis();

//                 while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
//                 {
//                     delay(300);
//                 }

//                 if (WiFi.status() == WL_CONNECTED)
//                 {
//                     return true;
//                 }
//             }
//         }
//     }
//     return false;
// }

// template <class Transport>
// inline void CKC_PnP<Transport>::CKC_state_Connect_AP()
// {
//     if (millis() - t2 > this->time_ap)
//     {
//         WiFi_TASK = MODE_STA;
//         CKC_LOG_DEBUG("WIFI", "RUN_STA");
//         CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
//         CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
//         t1 = millis();
//     }
//     else
//     {
//         if (millis() - t3 > 1000)
//         {
//             CKC_LOG_DEBUG("WIFI", "Waiting ___ %ds\r", (millis() - t2) / 1000);
//             t3 = millis();
//         }
//     }
//     if (this->CKC_connectAP())
//     {
//         WiFi_TASK = MODE_AP_STA;
//         CKC_LOG_DEBUG("WIFI", "RUN_STA_AP");
//     }
// }

// template <class Transport>
// inline void CKC_PnP<Transport>::Start_AP_CKC()
// {

//     WiFi_TASK = START_APCKC;
//     server->on("/", [this]()
//                { server->send(200, "text/html", htmlPage()); });
//     server->on("/save", [this]()
//                {
//         String ssid = server->arg("ssid");
//         String pass = server->arg("pass");

//         saveWiFi(ssid, pass);

//         server->send(200, "text/html", "Saved! Restart...");
//         delay(2000);
//         ESP.restart(); });
//     server->begin();
// }

// template <class Transport>
// inline void CKC_PnP<Transport>::CKC_mode_connected()
// {
//     serverMQTT.run();
//     if (!CkC_Connected())
//     {
//         WiFi_TASK = MODE_STA;
//         CKC_LOG_DEBUG("WIFI", "RUN_STA");
//         CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
//         CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
//         t1 = millis();
//     }
//     if (!serverMQTT._connect())
//     {
//         serverMQTT.begin();
//     }
// }

// template <class Transport>
// inline bool CKC_PnP<Transport>::CkC_Connected()
// {
//     if (WiFi.status() == WL_CONNECTED)
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

// template <class Transport>
// inline bool CKC_PnP<Transport>::CKC_connectAP()
// {
//     if (WiFi.softAPgetStationNum() > 0)
//     {
//         return true;  
//     }
//     else
//     {
//         return false;
//     }
// }

// template <class Transport>
// inline void CKC_PnP<Transport>::handler_button()
// {
// #ifdef BUTTON_MODE
//     bool pressed = (digitalRead(FLASH_BTN) == LOW); // nhấn = LOW

//     if (pressed)
//     {
//         if (pressStart == 0)
//             pressStart = millis();
//         // giữ đủ 5s và chỉ kích 1 lần
//         if (!triggered && (millis() - pressStart >= 5000))
//         {
//             triggered = true;
//             CKC_LOG_DEBUG("WIFI", "MODE_AP_run:");
//             CKC_LOG_DEBUG("WIFI", "WIFI_CONNECT_FALSE !!!!!! ");
//             CKC_LOG_DEBUG("WIFI", "RUN_AP");
//             CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
//             CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
//             CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
//             CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);
//             WiFi_TASK = MODE_AP;
//             WiFi.mode(WIFI_OFF);
//             WiFi.mode(WIFI_AP);
//             _ipAddr.fromString(_ap_ip);
//             WiFi.softAP(_ap_ssid, _ap_pass);
//             t2 = millis();
//             // TODO: đặt lệnh bạn muốn ở đây
//         }
//     }
//     else
//     {

//         // nhả nút thì reset lại
//         pressStart = 0;
//         triggered = false;
//     }

// #endif
// }

// template <class Transport>
// inline void CKC_PnP<Transport>::CKC_mode_Config()
// {
//     if (!CKC_connectAP())
//     {
//         /* code */
//     }
// };






