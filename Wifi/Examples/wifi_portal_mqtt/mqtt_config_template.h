#pragma once

// Copy to mqtt_config.h and enter the MQTT settings. mqtt_config.h is ignored
// by Git so real broker credentials are not accidentally committed.
#define MQTT_SERVER "maqiatto.com"
#define MQTT_PORT 1883
#define MQTT_USER "example@gmail.com"
#define MQTT_PASSWORD "example"
#define MQTT_CLIENT_ID "aqs-x01-esp01"
#define MQTT_CO2_TOPIC "example@gmail.com/co2"
#define MQTT_TEMP_TOPIC "example@gmail.com/temperature"
#define MQTT_HUM_TOPIC "example@gmail.com/humidity"
