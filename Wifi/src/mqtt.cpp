// PlatformIO entry point for the original Arduino example.
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
void setup_wifi();
void switchFan(bool fan_on);
#include "../Examples/mqtt/mqtt.ino"
