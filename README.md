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

- Sensirion SCD4X (Acustic CO2)- https://www.sensirion.com/products/catalog/SCD40
  Seamlessly compatible with https://www.tindie.com/products/modulefun/scd40-co2-sensor-breakout/

- Winsen MHZ19B (NDIR CO2) - https://www.winsen-sensor.com/sensors/co2-sensor/mh-z19b.html
- Sensirion SGP30 (TVOC) - https://www.sensirion.com/en/environmental-sensors/gas-sensors/sgp30/
- Sensirion SHT31 - https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/

Notes:  Winsen MHZ19C and Bosch BME680 in process to be tested.

###### Notes about NDIR CO2 and TVOC sensors:

For occupancy indication an NDIR based sensor is more reliable. A TVOC based sensor can give an idea about air quality if, a part of CO2 we are concern about other gases (e.g. cleaning products) that can be a plus depending on the application you want to cover.

Both sensors are ABC calibrated (https://www.co2meter.com/blogs/news/7512282-co2-sensor-calibration-what-you-need-to-know), so they need to be exposed to fresh air conditions every day. For the SGP30 the base is saved to non-volatile every 24h.

From Firmware version 1.1 manual calibration possibility has been added. Check manual calibration procedure.

###### Notes about measuring temperature and humidity:

Measuring temperature and humidity accurately inside an enclosure with different heat sources is not easy even if the temperature sensor position and design follows the "best practices notes". The binaries provided have a temperature offset to account for the board self-heating. The self-heating correction can be tweaked through Modbus. Correcting the temperature is a direct subtraction to the measured value. The humidity is also corrected applying the following formula:  

```
RH1*exp(4283.78*(t1-t2)/(243.12+t1)/(243.12+t2));
```

https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/2_Humidity_Sensors/Application_Note/Sensirion_Humidity_Sensors_at_a_Glance.pdf

Things to consider:

- The ESP8266-01 is a source of heat, when is in normal operation will increase the measured temperature for more than 3'C. This is why it is recommended to use the ESP8266-01 in sleep mode and the example provided is putting the ESP to deep sleep.

- The SGP30 is measuring the gases heating up a plate. That is generating heat and there is no sleep mode in this sensor other than removing power.

  If it is desired to have the SGP30 and the SHT31 in the same device, the temperature will have to be offset by several degrees.

In the current binary demos, the self-heating coefficient is a constant value to substract from the temperature read. If the self-heating factor changes related to the behavior of your application you will need to average it or find a complex way to track it and apply the correction to your device.

### Wifi

The AQS-X01 is compatible with the board ESP8266-01 (https://en.wikipedia.org/wiki/ESP8266)

### Modbus

Information related to Modbus. E.g. Modbus Registers.

## Firmware binaries

This section contains the binaries of the working demos for the device.

