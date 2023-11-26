import serial # type: ignore

from _typeshed import Incomplete

serialPortId: str    = '/dev/ttyUSB0' #Linux 
#serialPortId   = 'COM12' #Windows

deviceAddress: int   = 51 #default device Address AQS-X01 
baudrate: int        = 115200#default baudrate AQS-X01

parity: Incomplete          = serial.PARITY_NONE#default parity AQS-X01
stopbits: Incomplete        = serial.STOPBITS_ONE#default stopbits AQS-X01

timeout: float        = 0.1# seconds. At least 0.2 seconds required for 2400 bits/s.

delayMessages: float    = 0.1#seconds delay between register reads

pollingCycle:int    = 2#in seconds

