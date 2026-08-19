#include <Arduino.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "modbusCrc.h"
#include "modbusRegisters.h"
#include "mqtt_config.h"

namespace {
constexpr uint32_t SETTINGS_MAGIC = 0x41515331; // "AQS1"
constexpr uint32_t DOUBLE_RESET_MAGIC = 0x44525354; // "DRST"
constexpr size_t EEPROM_BYTES = 128;
constexpr uint32_t WIFI_TIMEOUT_MS = 20000;
constexpr uint32_t RESET_WINDOW_MS = 2500;
constexpr uint16_t DNS_PORT = 53;
constexpr char AP_PASSWORD[] = "configureme";

struct WifiSettings {
  uint32_t magic;
  char ssid[33];
  char password[65];
  uint32_t checksum;
};

ESP8266WebServer webServer(80);
DNSServer dnsServer;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
WifiSettings settings{};
bool apMode = false;

bool doubleResetRequested() {
  uint32_t marker = 0;
  ESP.rtcUserMemoryRead(0, &marker, sizeof(marker));
  if (marker == DOUBLE_RESET_MAGIC) {
    marker = 0;
    ESP.rtcUserMemoryWrite(0, &marker, sizeof(marker));
    return true;
  }

  marker = DOUBLE_RESET_MAGIC;
  ESP.rtcUserMemoryWrite(0, &marker, sizeof(marker));
  delay(RESET_WINDOW_MS);
  marker = 0;
  ESP.rtcUserMemoryWrite(0, &marker, sizeof(marker));
  return false;
}

uint32_t settingsChecksum(const WifiSettings &value) {
  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
  uint32_t result = 2166136261UL;
  for (size_t i = 0; i < offsetof(WifiSettings, checksum); ++i) {
    result = (result ^ bytes[i]) * 16777619UL;
  }
  return result;
}

bool loadSettings() {
  EEPROM.get(0, settings);
  settings.ssid[sizeof(settings.ssid) - 1] = '\0';
  settings.password[sizeof(settings.password) - 1] = '\0';
  return settings.magic == SETTINGS_MAGIC &&
         settings.checksum == settingsChecksum(settings) &&
         settings.ssid[0] != '\0';
}

void saveSettings(const String &ssid, const String &password) {
  memset(&settings, 0, sizeof(settings));
  settings.magic = SETTINGS_MAGIC;
  ssid.toCharArray(settings.ssid, sizeof(settings.ssid));
  password.toCharArray(settings.password, sizeof(settings.password));
  settings.checksum = settingsChecksum(settings);
  EEPROM.put(0, settings);
  EEPROM.commit();
}

void clearSettings() {
  memset(&settings, 0, sizeof(settings));
  EEPROM.put(0, settings);
  EEPROM.commit();
}

String apName() {
  return String("AQS-X01-Setup-") + String(ESP.getChipId(), HEX);
}

String htmlEscape(const String &input) {
  String output;
  output.reserve(input.length() + 8);
  for (size_t i = 0; i < input.length(); ++i) {
    switch (input[i]) {
      case '&': output += F("&amp;"); break;
      case '<': output += F("&lt;"); break;
      case '>': output += F("&gt;"); break;
      case '\"': output += F("&quot;"); break;
      case '\'': output += F("&#39;"); break;
      default: output += input[i]; break;
    }
  }
  return output;
}

String page(const String &message = String()) {
  String html;
  html.reserve(3000);
  html += F("<!doctype html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>"
            "<title>AQS-X01 Wi-Fi</title><style>body{font-family:sans-serif;max-width:32rem;margin:3rem auto;padding:0 1rem}"
            "input,select,button{box-sizing:border-box;width:100%;padding:.8rem;margin:.35rem 0}"
            ".msg{padding:.8rem;background:#eef;border-radius:.4rem}.danger{background:#b22;color:white}</style></head><body>"
            "<h1>AQS-X01 Wi-Fi</h1>");
  if (message.length()) html += String("<p class='msg'>") + htmlEscape(message) + F("</p>");
  html += F("<form method='post' action='/configure'><label>Detected networks</label>"
            "<select id='network' onchange=\"document.getElementById('ssid').value=this.value\">"
            "<option value=''>-- Select a Wi-Fi network --</option>");

  const int networkCount = WiFi.scanNetworks(false, true);
  for (int i = 0; i < networkCount; ++i) {
    const String security = WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "secured";
    html += String("<option value=\"") + htmlEscape(WiFi.SSID(i)) + "\">" +
            htmlEscape(WiFi.SSID(i)) + " (" + WiFi.RSSI(i) + " dBm, " + security + F(")</option>");
  }
  WiFi.scanDelete();

  html += F("</select>");
  if (networkCount <= 0) {
    html += F("<p class='msg'>No networks found. Press Scan again or enter the SSID manually.</p>");
  }
  html += F("<label>Wi-Fi SSID</label>"
            "<input id='ssid' name='ssid' maxlength='32' required autocomplete='off'>"
            "<small>Select a detected network above or type a hidden SSID manually.</small>"
            "<label>Password</label>"
            "<input name='password' type='password' maxlength='64' autocomplete='off'>"
            "<button type='submit'>Test and save connection</button></form>"
            "<p><a href='/'>Scan again</a></p>"
            "<form method='post' action='/forget' onsubmit=\"return confirm('Forget the saved Wi-Fi?')\">"
            "<button class='danger' type='submit'>Forget saved Wi-Fi</button></form>");
  if (WiFi.status() == WL_CONNECTED) {
    html += String("<p>Connected to <strong>") + htmlEscape(WiFi.SSID()) +
            F("</strong><br>Device address: http://") + WiFi.localIP().toString() + F("/</p>");
  } else if (apMode) {
    html += String("<p>Setup network: <strong>") + apName() + F("</strong><br>Setup address: http://192.168.4.1/</p>");
  }
  html += F("</body></html>");
  return html;
}

bool connectWifi(const char *ssid, const char *password) {
  WiFi.mode(apMode ? WIFI_AP_STA : WIFI_STA);
  WiFi.begin(ssid, password);
  const uint32_t started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < WIFI_TIMEOUT_MS) {
    delay(100);
    yield();
  }
  return WiFi.status() == WL_CONNECTED;
}

void startAccessPoint() {
  apMode = true;
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apName().c_str(), AP_PASSWORD);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
}

void handleConfigure() {
  const String newSsid = webServer.arg("ssid");
  const String newPassword = webServer.arg("password");
  if (!newSsid.length() || newSsid.length() > 32 || newPassword.length() > 64) {
    webServer.send(400, "text/html", page("Invalid SSID or password length."));
    return;
  }

  if (!connectWifi(newSsid.c_str(), newPassword.c_str())) {
    startAccessPoint();
    webServer.send(200, "text/html", page("Connection failed. Check the credentials and try again."));
    return;
  }

  saveSettings(newSsid, newPassword);
  webServer.send(200, "text/html", page("Connection succeeded. Credentials saved; restarting now."));
  delay(1200);
  ESP.restart();
}

void handleForget() {
  clearSettings();
  webServer.send(200, "text/html", page("Saved Wi-Fi removed. Restarting in setup mode."));
  delay(1200);
  ESP.restart();
}

void startWebServer() {
  webServer.on("/", HTTP_GET, [] { webServer.send(200, "text/html", page()); });
  webServer.on("/configure", HTTP_POST, handleConfigure);
  webServer.on("/forget", HTTP_POST, handleForget);
  webServer.onNotFound([] {
    if (apMode) {
      webServer.sendHeader("Location", "http://192.168.4.1/", true);
      webServer.send(302, "text/plain", "");
    } else {
      webServer.send(404, "text/plain", "Not found");
    }
  });
  webServer.begin();
}

uint16_t calculateCRC(uint8_t *message, uint8_t length) {
  uint8_t crcLsb = 0xFF;
  uint8_t crcMsb = 0xFF;
  for (uint8_t i = 0; i < length; ++i) {
    const uint16_t index = crcLsb ^ message[i];
    crcLsb = crcMsb ^ crcHighTable[index];
    crcMsb = crcLowTable[index];
  }
  return (crcMsb << 8) | crcLsb;
}

bool publishAqsData() {
  uint8_t request[] = {0x33, 0x04, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00};
  const uint16_t crc = calculateCRC(request, 6);
  request[6] = uint8_t(crc);
  request[7] = uint8_t(crc >> 8);
  Serial.write(request, sizeof(request));

  constexpr uint8_t MAX_REPLY = 50;
  uint8_t reply[MAX_REPLY];
  const uint8_t received = Serial.readBytes(reply, MAX_REPLY);
  // Function 4 reply for 13 registers: address + function + byte count +
  // 26 data bytes + CRC.
  if (received < 31) return false;
  const uint16_t messageCrc = (uint16_t(reply[received - 1]) << 8) | reply[received - 2];
  if (messageCrc != calculateCRC(reply, received - 2)) return false;

  const uint16_t co2 = (uint16_t(reply[3 + InputRegisterCo2Avg * 2]) << 8) |
                       reply[4 + InputRegisterCo2Avg * 2];
  const uint16_t tempRaw = (uint16_t(reply[3 + InputRegisterTemp * 2]) << 8) |
                           reply[4 + InputRegisterTemp * 2];
  const uint16_t humRaw = (uint16_t(reply[3 + InputRegisterHum * 2]) << 8) |
                          reply[4 + InputRegisterHum * 2];
  char value[16];
  snprintf(value, sizeof(value), "%u", co2);
  bool published = mqttClient.publish(MQTT_CO2_TOPIC, value, true);
  snprintf(value, sizeof(value), "%.2f", tempRaw / 100.0f);
  published = mqttClient.publish(MQTT_TEMP_TOPIC, value, true) && published;
  snprintf(value, sizeof(value), "%.2f", humRaw / 100.0f);
  published = mqttClient.publish(MQTT_HUM_TOPIC, value, true) && published;
  return published;
}

void maintainMqtt() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (!mqttClient.connected()) {
    mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD);
  }
  mqttClient.loop();
}

void maintainWifiFallback() {
  static uint32_t disconnectedAt = 0;
  if (WiFi.status() == WL_CONNECTED || apMode) {
    disconnectedAt = 0;
    return;
  }
  if (disconnectedAt == 0) disconnectedAt = millis();
  if (millis() - disconnectedAt >= WIFI_TIMEOUT_MS) {
    startAccessPoint();
    startWebServer();
  }
}
} // namespace

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  EEPROM.begin(EEPROM_BYTES);
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

  const bool haveSettings = loadSettings();
  if (!haveSettings) {
    startAccessPoint();
    startWebServer();
    return;
  }

  // An ordinary AQS wake produces one reset. Two resets within this short
  // startup window explicitly request the configuration portal.
  if (doubleResetRequested()) {
    startAccessPoint();
    startWebServer();
    return;
  }

  if (!connectWifi(settings.ssid, settings.password)) {
    startAccessPoint();
    startWebServer();
  }
}

void loop() {
  if (apMode) {
    webServer.handleClient();
    dnsServer.processNextRequest();
    return;
  }
  maintainWifiFallback();

  static uint32_t lastMqttAttempt = 0;
  if (millis() - lastMqttAttempt >= 1000) {
    lastMqttAttempt = millis();
    maintainMqtt();
  }

  static uint32_t lastAqsRead = 0;
  if (!apMode && WiFi.status() == WL_CONNECTED && mqttClient.connected() && millis() - lastAqsRead >= 1000) {
    lastAqsRead = millis();
    if (publishAqsData()) {
      mqttClient.loop();
      delay(500); // allow the TCP stack to transmit before power-down
      ESP.deepSleep(0); // sleep indefinitely; AQS-X01 wakes the ESP through RESET
    }
  }
}
