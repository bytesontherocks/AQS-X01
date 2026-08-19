// PlatformIO entry point for the original Arduino example.
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
void setup_wifi();
enum class RemoteDoState : bool;
void switchDO(RemoteDoState do_state);
#include "../Examples/remote_DO/remote_DO.ino"
