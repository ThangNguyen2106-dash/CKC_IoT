#ifndef CKC_WEBUI_HPP
#define CKC_WEBUI_HPP

#include <Arduino.h>

class CKC_WebUI
{
public:
    static const char WebConfigHEAD[];
    static const char WebConfigFOOT[];

    static String buildWiFiOption(const String &ssid, int rssi);
    static int getSignalLevel(int rssi);
};

// =====================================================
// HTML HEAD
// =====================================================
const char CKC_WebUI::WebConfigHEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="UTF-8">
<title>WiFi CONFIG</title>

<meta name="viewport"
      content="width=device-width, initial-scale=1.0">

<style>
  :root {
            --primary: #4CAF93;
            --bg: #f5f7f9;
            --card: #ffffff;
            --text: #2c3e50;
            --border: #e0e6ed;
        }

        body {
            font-family: "Segoe UI", sans-serif;
            background: var(--bg);
            margin: 0;
            padding: 20px;
            color: var(--text);
        }

        .container {
            max-width: 400px;
            margin: auto;
        }

        .card {
            background: var(--card);
            padding: 25px;
            border-radius: 14px;
            box-shadow: 0 6px 20px rgba(0,0,0,0.05);
        }

        h2 {
            margin-bottom: 20px;
            text-align: center;
        }

        label {
            display: block;
            margin-top: 12px;
            font-weight: 500;
        }

        select,
        input {
            width: 100%;
            padding: 12px;
            border-radius: 10px;
            border: 1px solid var(--border);
            margin-top: 8px;
            box-sizing: border-box;
        }

        button,
        input[type="submit"] {
            width: 100%;
            padding: 12px;
            border-radius: 10px;
            border: none;
            background: var(--primary);
            color: white;
            margin-top: 15px;
            cursor: pointer;
            font-size: 15px;
        }

        hr {
            border: none;
            border-top: 1px solid var(--border);
            margin: 25px 0;
        }

        .select-wrap {
            position: relative;
        }

        #signalView {
            position: absolute;
            right: 35px;
            top: 50%;
            transform: translateY(-50%);
            pointer-events: none;
        }

        .signal {
            display: flex;
            gap: 3px;
            align-items: flex-end;
        }

        .bar {
            width: 5px;
            border-radius: 3px;
            background: #d0d7de;
        }

        .bar.active {
            background: var(--primary);
        }

        .bar:nth-child(1) { height: 6px; }
        .bar:nth-child(2) { height: 10px; }
        .bar:nth-child(3) { height: 14px; }
        .bar:nth-child(4) { height: 18px; }

        .level-1 .bar:nth-child(n+2),
        .level-2 .bar:nth-child(n+3),
        .level-3 .bar:nth-child(4) {
            background: #d0d7de !important;
        }
       .password-box {
    position: relative;
    width: 100%;
}

.password-box input {
    width: 100%;
    padding-right: 45px;
}

.toggle-eye {
    position: absolute;
    right: 14px;
    top: 50%;
    transform: translateY(-50%);
    cursor: pointer;
    user-select: none;
    font-size: 16px;
    opacity: 0.7;
}

.toggle-eye:hover {
    opacity: 1;
</style>
</head>

<body>

<div class="container">

<div class="card">

<h2>WIFI & MQTT CONFIG</h2>

<form action="/connect" method="POST">

<label>SSID</label>

<div class="select-wrap">

<select id="wifiSelect" name="ssid">
)rawliteral";

// =====================================================
// HTML FOOT
// =====================================================
const char CKC_WebUI::WebConfigFOOT[] PROGMEM = R"rawliteral(
</select>

<div id="signalView"></div>

</div>

<label>WiFi Password</label>

<div class="password-box">

    <input type="password"
           id="wifiPass"
           name="pass"
           placeholder="Enter WiFi password">

    <span class="toggle-eye"
          onclick="togglePassword('wifiPass', this)">
        👁
    </span>

</div>

<label>MQTT Username</label>

<input type="text"
       name="mqtt_user"
       placeholder="Enter MQTT username">

<label>MQTT Password</label>

<div class="password-box">

    <input type="password"
           id="mqttPass"
           name="mqtt_pass"
           placeholder="Enter MQTT password">

    <span class="toggle-eye"
          onclick="togglePassword('mqttPass', this)">
        👁
    </span>

</div>

<input type="submit"
       value="CONNECT">

</form>

<hr>

<form action="/scan" method="GET">

<button type="submit">
    RELOAD WIFI
</button>

</form>

</div>

</div>

<script>

const select = document.getElementById("wifiSelect");
const signalView = document.getElementById("signalView");

function renderSignal(level)
{
    let html =
        '<div class="signal level-' + level + '">';

    for (let i = 0; i < 4; i++)
    {
        html += '<span class="bar active"></span>';
    }

    html += '</div>';

    signalView.innerHTML = html;
}

select.addEventListener("change", function ()
{
    let level =
        this.options[this.selectedIndex]
        .getAttribute("data-level");

    renderSignal(level);
});

window.onload = function ()
{
    if (select.options.length > 0)
    {
        let level =
            select.options[select.selectedIndex]
            .getAttribute("data-level");

        renderSignal(level);
    }
};

function togglePassword(inputId, eye)
{
    const input = document.getElementById(inputId);

    if (input.type === "password")
    {
        input.type = "text";
        eye.innerHTML = "👁";
    }
    else
    {
        input.type = "password";
        eye.innerHTML = "👁";
    }
}

</script>

</body>
</html>


)rawliteral";

// =====================================================
// SIGNAL LEVEL CALC
// =====================================================
int CKC_WebUI::getSignalLevel(int rssi)
{
    if (rssi > -60)
        return 4;
    if (rssi > -70)
        return 3;
    if (rssi > -80)
        return 2;
    return 1;
}

// =====================================================
// BUILD OPTION HTML
// =====================================================
String CKC_WebUI::buildWiFiOption(const String &ssid, int rssi)
{
    int level = getSignalLevel(rssi);

    String option = "<option value='";
    option += ssid;
    option += "' data-level='";
    option += String(level);
    option += "'>";
    option += ssid;
    option += " (";
    option += String(rssi);
    option += " dBm)</option>";
    return option;
}

#endif