#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ModbusCrc.h"
#include "ModbusRegisters.h"
#include "credentials.h"
#include <String>

// Replace by 2 if you aren't enable to use Serial Monitor... Don't forget to Rewire R1 to GPIO2!
#define in_led 0

String temp_str;
String hum_str;
char tempArray[10];
char humArray[10];

WiFiClient espClient;
HTTPClient http;

enum class RemoteDoState : bool
{
  DO_OFF = false,
  DO_ON = true
};

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);//ms
  setup_wifi();

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(in_led, OUTPUT);
  digitalWrite(in_led, HIGH);
}

void setup_wifi() {
  delay(100);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
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


const std::uint16_t min_level = 1000;
const std::uint16_t max_level = 1500;

// because we don't keep power to the board we don't have the previous state of the system
// bool from_max_level = false;
// void controlRemoteOutput(const std::uint16_t level)
// {
//   if (from_max_level)
//   {
//     if (level < min_level)
//     {
//       switchDO(false);
//       from_max_level = false;

//     }
//   } else {
//     if (level > max_level)
//     {
//       switchDO(true);
//       from_max_level = true;
//     }
//   }
// }


void controlRemoteOutput(const std::uint16_t level)
{
  if (level > max_level)
  {
    switchDO( RemoteDoState::DO_ON);
  } else
  {
    if (level < min_level)
    {
      switchDO( RemoteDoState::DO_OFF);
    }
  }
}

// Remote DO hardware specific: for this particular case Sonoff mini r2
void switchDO(const RemoteDoState do_state)
{
  // e.g. "http://192.168.1.10:8081/zeroconf/switch"
  String url_do_post = String("http://") + String(sonoff_ip_port) + String("/zeroconf/switch");

  http.begin(espClient, url_do_post);
  http.addHeader("Content-Type", "application/json");
  String sonoff_command = "";

  if (do_state == RemoteDoState::DO_ON)
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
      controlRemoteOutput(co2Avrg); // Fan control

      // allow finishing sending messages before shutting down. Would be better to have feedback from the library instead of open loop delay
      delay(1000);

	    // Go to sleep for 'Esp8266HoldResetPeriod' seconds (see Modbus Register documentation), when the period elapses the AQS will wake the ESP8266 up
      ESP.deepSleep(0);
    }
  }
}

void loop()
{
  //get data from the AQS and go to deep sleep after publishing the data
  getDataFromAqs();

  // If data has been obtained and 'Esp8266HoldResetPeriod' seconds > than 0 we shouldn't reach this point because we are sleeping.
  // However, if something hasn't work, retry in 1s
  delay(1000);
}
