import serial

serialPortId    = '/dev/ttyUSB0' #Linux
#serialPortId   = 'COM12' #Windows

deviceAddress   = 51 #default device Address AQS-X01 
baudrate        = 115200#default baudrate AQS-X01

parity          = serial.PARITY_NONE#default parity AQS-X01
stopbits        = serial.STOPBITS_ONE#default stopbits AQS-X01

timeout         = 0.1# seconds. At least 0.2 seconds required for 2400 bits/s.

delayMessages   = 0.1#seconds delay between register reads

pollingCycle    = 2#in seconds
