### Using Sonoff mini r2

## Overview
In this example a remote relay is switch on and off according to the level of average CO2. This relay controls an extractor fan in a room, so when a level is superior to certain threshold (e.g. 1500 ppm) the fan is switched on until the level of CO2 gets down to certain threshold (e.g. 1000 ppm) acting like a hysteresis control.

## Sonoff mini r2
This device in diy mode allows to switch on and off the relay just with a simple http post request and for this simple use case that is enough:

HTTP POST: "http://192.168.1.10:8081/zeroconf/switch"

Raw body:

```json
{
    "deviceid": "",
    "data": {
        "switch": "off"
    }
}
```
```json
{
    "deviceid": "",
    "data": {
        "switch": "on"
    }
}
```

Using the library "HttpClient" from Arduino the request is effective and simple. See https://reference.arduino.cc/reference/en/libraries/httpclient/
