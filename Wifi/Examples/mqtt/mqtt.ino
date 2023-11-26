#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include "ModbusCrc.h"
#include "ModbusRegisters.h"
#include "credentials.h"
#include "mqtt.h"
#include <String>

// Replace by 2 if you aren't enable to use Serial Monitor... Don't forget to Rewire R1 to GPIO2!
#define in_led 0

String temp_str;
String hum_str;
char tempArray[10];
char humArray[10];

WiFiClient espClient;
HTTPClient http;
PubSubClient client;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);//ms
  setup_wifi();
  // client.setClient(espClient);
  // client.setServer(mqtt_server, mqtt_port);
  // client.setCallback(callback);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(in_led, OUTPUT);
  digitalWrite(in_led, HIGH);
}
//
void setup_wifi() {
  delay(100);
  // We start by connecting to a WiFi network
  //Serial.println();
   //Serial.print("Connecting to ");
  //Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     //Serial.print("wifi not connected... trying again in 500ms");
  }

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());

  //switch fan off as initialisation
  //switchFan(false);
}

void reconnect() {
  // Loop until we're reconnected//
  while (!client.connected()) {
     //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    if (client.connect(uniqueClientID)) {
    //if (client.connect(uniqueClientID, mqtt_user, mqtt_password)) {
      //Serial.println("connected");
    } else {
       //Serial.print("failed, rc=");
       //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
 for (int i = 0; i < length; i++) {
  char receivedChar = (char)payload[i];
   //Serial.print(receivedChar);
  if (receivedChar == '0')
   digitalWrite(in_led, LOW);
  if (receivedChar == '1')
   digitalWrite(in_led, HIGH);
 }
 //Serial.println();
}

uint16_t calculateCRC(uint8_t* message, uint8_t messageLength)
{
  uint8_t crcLsb = 0xFF;
  uint8_t crcMsb = 0xFF;
  uint8_t messageIndex = 0;

  while( messageLength > 0 ) {
    uint16_t tableIndex = crcLsb ^ message[messageIndex];
    crcLsb =  crcMsb ^ crcHighTable[tableIndex];
    crcMsb = crcLowTable[tableIndex];
    messageIndex++;
    messageLength--;
  }

  return( (crcMsb << 8) | crcLsb );
}

bool from_max_level = false;
const std::uint16_t min_level = 1000;
const std::uint16_t max_level = 1500;

// void controlHisteresisFan(const std::uint16_t level)
// {
//   if (from_max_level)
//   {
//     if (level < min_level)
//     {
//       switchFan(false);
//       from_max_level = false;

//     }
//   } else {
//     if (level > max_level)
//     {
//       switchFan(true);
//       from_max_level = true;
//     }
//   }
// }


void controlHisteresisFan(const std::uint16_t level)
{
  if (level > max_level)
  {
    switchFan(true);
  } else
  {
    if (level < min_level)
    {
      switchFan(false);
    }
  }
}
void switchFan(bool fan_on)
{
  // Your Domain name with URL path or IP address with path
  http.begin(espClient, "http://192.168.4.146:8081/zeroconf/switch");
  http.addHeader("Content-Type", "application/json");
  String sonoff_command = "";
  if (fan_on)
  {
    sonoff_command = "{\"deviceid\":\"\",\"data\":{\"switch\":\"on\"}}";
    //Serial.println("Switch fan on");
  } else {
    sonoff_command = "{\"deviceid\":\"\",\"data\":{\"switch\":\"off\"}}";
    //Serial.println("Switch fan off");
  }

  int httpResponseCode = http.POST(sonoff_command);

   //Serial.print("HTTP Response code: ");
  //Serial.println(httpResponseCode);

  // Free resources
  http.end();
}

//get data from the AQS following the Modbus RTU protocol.
void getDataFromAqs(void)
{
	// Slave address (ignored), Function Code (FC4- Input Registers), Starting Address MSB /LSB, Quantity of Registers MSB/ LSB, CRC MSB /LSB
  uint8_t request[] = {0x33, 0x04, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00};

  // calculate message CRC and send query
  uint16_t crc = calculateCRC(request, 6);
  request[6] = (uint8_t)(crc);
  request[7] = (uint8_t)(crc >> 8);
  Serial.write(request, sizeof(request));

  #define maxReplyBytes 50
  static uint8_t reply[maxReplyBytes];
  uint8_t readBytes = Serial.readBytes(reply, maxReplyBytes);

  if (readBytes) {

    uint16_t crcInMessage = (reply[readBytes-1] << 8) + reply[readBytes-2];
    uint16_t crcInCalculated = calculateCRC(reply, readBytes - 2);

    if (crcInMessage == crcInCalculated) {

      uint16_t co2        = (reply[3 + (InputRegisterCo2*2)]<<8) + reply[4 + (InputRegisterCo2*2)];
      uint16_t co2Avrg    = (reply[3 + (InputRegisterCo2Avg*2)]<<8) + reply[4 + (InputRegisterCo2Avg*2)];
      uint16_t temp       = (reply[3 + (InputRegisterTemp*2)]<<8) + reply[4 + (InputRegisterTemp*2)];
      uint16_t hum        = (reply[3 + (InputRegisterHum*2)]<<8)   + reply[4 + (InputRegisterHum*2)];
      uint16_t tvoc       = (reply[3 + (InputRegisterTvoc*2)]<<8) + reply[4 + (InputRegisterTvoc*2)];
      uint16_t eCo2       = (reply[3 + (InputRegistereCo2*2)]<<8) + reply[4 + (InputRegistereCo2*2)];
      uint16_t tvocAvg    = (reply[3 + (InputRegisterTvocAvg*2)]<<8) + reply[4 + (InputRegisterTvocAvg*2)];
      uint16_t eCo2Avg    = (reply[3 + (InputRegistereCo2Avg*2)]<<8) + reply[4 + (InputRegistereCo2Avg*2)];
      uint16_t tvocBase   = (reply[3 + (InputRegisterTvocBase*2)]<<8) + reply[4 + (InputRegisterTvocBase*2)];
      uint16_t eCo2Base   = (reply[3 + (InputRegistereCo2Base*2)]<<8) + reply[4 + (InputRegistereCo2Base*2)];
      uint16_t ethanolRaw = (reply[3 + (InputRegisterEthanolRaw*2)]<<8) + reply[4 + (InputRegisterEthanolRaw*2)];
      uint16_t H2Raw      = (reply[3 + (InputRegisterH2Raw*2)]<<8) + reply[4 + (InputRegisterH2Raw*2)];
      uint16_t fwVersion  = (reply[3 + (InputRegisterFwVersion*2)]<<8)   + reply[4 + (InputRegisterFwVersion*2)];

      float fTemp = temp;
      fTemp /= 100;
      float fHum = hum;
      fHum /= 100;

      temp_str = String(fTemp);
      temp_str.toCharArray(tempArray, temp_str.length() + 1);

      hum_str = String(fHum);
      hum_str.toCharArray(humArray, hum_str.length() + 1);

      //Serial.println("C02 level: ");
      //Serial.println(eCo2Avg);
      controlHisteresisFan(co2Avrg); // Fan control

      // Add here what data you want to publish
      // client.publish(co2_topic, String(co2Avrg).c_str(), true);
      // client.publish(temp_topic, tempArray);
      // client.publish(hum_topic, humArray);

      // allow finishing sending messages before shutting down. Would be better to have feedback from the library instead of open loop delay
      delay(1000);

	    // Go to sleep for 'Esp8266HoldResetPeriod' seconds (see Modbus Register documentation), when the period elapses the AQS will wake the ESP8266 up
      ESP.deepSleep(0);
    }
  }
}

void loop()
{

  // static bool change_relay = false;
  // static std::uint16_t co2_level = 400;
  // static std::int16_t co2_level_increase = 100;
  // //Serial.println("C02 level: ");
  // //Serial.println(std::to_string(co2_level).c_str());
  // //Serial.println("...");
  // controlHisteresisFan(co2_level); // Fan control

  // if (co2_level > 2000)
  //   co2_level_increase = -100;
  // else if (co2_level < 400)
  //   co2_level_increase = 100;

  // co2_level += co2_level_increase;

  //get data from the AQS and go to deep sleep after publishing the data
  getDataFromAqs();

  // If data has been obtaine and 'Esp8266HoldResetPeriod' seconds > than 0 we shouldn't reach this point because we are sleeping.
  // However, if something hasn't work, retry in 1s
  delay(1000);
}
