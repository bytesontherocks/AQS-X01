# 				AQS-X01 - Modbus RTU

## Serial Port Configuration

- Baudrate: 115200 bits/s
- Stop bits: 1
- Parity: None

## Registers

### FC4 - Input Registers

| Address | Name         | Description                                            | Range     | Units     |
| ------- | ------------ | ------------------------------------------------------ | --------- | --------- |
| 0       | CO2          | Value read from the NDIR CO2 sensor (updated every 5s) | 400-2000  | ppm       |
| 1       | CO2 Average  | 1 minute rolling average from NDIR CO2 value           | 400-2000  | ppm       |
| 2       | Temperature  | Value read from the SHT31                              | 0-9000    | 'C (x100) |
| 3       | Humidity     | Value read from the SHT31                              | 0-10000   | %(x100)   |
| 4       | TVOC         | Value read from the MOX sensor (every 1s)              | 0-60000   | ppb       |
| 5       | eCO2         | Value read from the MOX sensor (every 1s)              | 400-60000 | ppm       |
| 6       | TVOC Average | 1 minute rolling average from the MOX TVOC value       | 0-60000   | ppb       |
| 7       | eCO2 Average | 1 minute rolling average from the MOX eCO2 value       | 400-60000 | ppm       |
| 8       | TVOC base    | Value read from the MOX sensor (every 12h)             |           |           |
| 9       | eCO2 base    | Value read from the MOX sensor (every 12h)             |           |           |
| 10      | Ethanol Raw  | Value read from the MOX sensor (every 1s)              |           | ppm       |
| 11      | H2 Raw       | Value read from the MOX sensor (every 1s)              |           | ppm       |
| 12      | FwVersion    | Reports the firmware version of the device             |           | MM.mm     |



### FC3/FC6 - Holding Registers

The holding registers are written in non-volatile. After editing them allow 10 seconds for the parameters to be saved and then power cycle the device.

| Address | Name                    | Description                                                  | Range  | Units         | Default             |
| ------- | ----------------------- | ------------------------------------------------------------ | ------ | ------------- | ------------------- |
| 0       | GreenAmberThreshold     | Defines threshold for Bicolour LED to go from green to amber | 0-2000 | ppm           | 800                 |
| 1       | AmberRedThreshold       | Defines threshold for Bicolour LED to go from amber to red   | 0-2000 | ppm           | 1500                |
| 2       | SlaveAddress            | Modbus RTU Device address                                    | 1-248  |               | 51                  |
| 3       | ModbusRTUSerialBaudrate | Modbus RTU serial port baudrate                              |        | bits/s (/100) | 1152 (115200bits/s) |
| 4       | ModbusRTUSerialConf     | Modbus RTU serial port stop bit and parity                   |        |               | 0x0100              |
| 5       | Sht31TemperatureOffset  | Compensation due to device self-heating                      |        | 'C(x100)      | 200 (2'C)           |
| 6       | Esp8266HoldResetPeriod  | Period elapsed between successful replied message from ESP8622 and ESP8622 reset |        | s             | 300                 |



## Changing the Serial Port Configuration

- To change the baudrate write at this ModbusRTUSerialBaudrate register with the baudrate divided by 100. E.g. For 38400bit/s write 384. For 9600 bit/s write 96. etc.
- Changing stop bits and parity. The default value is 0x0100. 
  - The Most Significative Byte is the Stop bit. 0x02 is 2 stop bits. 0x01 or whatever other value except 0x02 is 1 stop bit.
  - The Less Sigificative Byte is the Parity. 
    - 0x00: None (or any other value except 0x01 and 0x02)
    - 0x01: Odd
    - 0x02: Even



## Changing the ESP8266 reset period

This parameter is ignored if the ESP8266 is not connected.

If set to 0 the reset is disabled.

See further documentation for details.

Look for extended document about how the AQS-X01 handles the communications with ESP8266. Basically, the power consumption of the ESP8266 heats up the board making the temperature and humidity reading offset by several degrees. However, if the ESP8266 is placed in deep sleep after receiving a message from the AQS-X01 

