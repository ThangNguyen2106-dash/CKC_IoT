#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
String htmlPage()
{
  return R"rawliteral(
    <html>
    <body>
      <h2>WiFi Config</h2>
      <form action="/save">
        SSID:<br>
        <input name="ssid"><br>
        PASS:<br>
        <input name="pass"><br><br>
        <input type="submit" value="Save">
      </form>
    </body>
    </html>
  )rawliteral";
}