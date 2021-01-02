# AQS-X01
Material related to AQS-X01 device: https://www.tindie.com/products/bytesontherocks/air-quality-platform-co2/

## Material

### Board and microcontroller

Find the board schematics in this section.

The AQS-X01 is based on an STM32G0 microcontroller. To start developing code the CubeMX tool can be used to generate code automatically. The CubeMx project can be found in the hardware section. (*.ioc). A multiplatform IDE based on Eclipse and GCC is also available  from ST (STM32IDE) - it can be used to compile and debug (with st-link, j-link, etc.) 

https://www.st.com/en/development-tools/stm32cubemx.html

https://www.st.com/en/development-tools/stm32cubeide.html

### Sensors

The following sensors are used in the project. Multiple open-source libraries can be found if you want to develop your own code.

- Winsen MHZ19B (NDIR CO2) - https://www.winsen-sensor.com/sensors/co2-sensor/mh-z19b.html
- Sensirion SGP30 (TVOC) - https://www.sensirion.com/en/environmental-sensors/gas-sensors/sgp30/
- Sensirion SHT31 - https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/

Notes:  Winsen MHZ19C and Bosch BME680 in process to be tested.

###### Notes about NDIR CO2 and TVOC sensors:

For occupancy indication an NDIR based sensor is more reliable. A TVOC based sensor can give an idea about air quality if, a part of CO2 we are concern about other gases (e.g. cleaning products) that can be a plus depending on the application you want to cover.

Both sensors are ABC calibrated (https://www.co2meter.com/blogs/news/7512282-co2-sensor-calibration-what-you-need-to-know), so they need to be exposed to fresh air conditions every day. For the SGP30 the base is saved to non-volatile every 24h.

### Wifi

The AQS-X01 is compatible with the board ESP8266-01 (https://en.wikipedia.org/wiki/ESP8266)

### Modbus

Information related to Modbus. E.g. Modbus Registers.



## Firmware binaries

This section contains the binaries of the working demos for the device.

