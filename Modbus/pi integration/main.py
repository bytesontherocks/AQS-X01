#AQS-X01 @ BytesOnTheRocks

import os
import sys
import time

import minimalmodbus

import Configuration
import Sht31
import Co2
import Sgp30
import AQSX01_Conf

# Allow long also in Python3
# http://python3porting.com/noconv.html
if sys.version > '3':
    long = int

def main():

    ################################
    ## Create instrument instance ##
    ################################
    instrument = minimalmodbus.Instrument(Configuration.serialPortId, Configuration.deviceAddress, minimalmodbus.MODE_RTU)
    instrument.serial.baudrate = Configuration.baudrate
    instrument.serial.timeout = Configuration.timeout
    instrument.serial.parity  = Configuration.parity
    instrument.serial.stopbits = Configuration.stopbits    
    instrument.debug = False
    instrument.precalculate_read_size = True


    text = '\n'
    text += '###############################################################\n'
    text += '## Hardware test AQS                                     ##\n'
    text += '## Minimalmodbus version: {:8}                           ##\n'.format(minimalmodbus.__version__)
    text += '##                                                           ##\n'
    text += '## Modbus mode:    {:15}                           ##\n'.format(instrument.mode)
    text += '## Python version: {}.{}.{}                                     ##\n'.format(sys.version_info[0], sys.version_info[1], sys.version_info[2])
    text += '## Baudrate (-b):  {:>5} bits/s                              ##\n'.format(instrument.serial.baudrate)
    text += '## Platform:       {:15}                           ##\n'.format(sys.platform)
    text += '##                                                           ##\n'
    text += '## Port name (-D): {:15}                           ##\n'.format(instrument.serial.port)
    text += '## Slave address:  {:<15}                           ##\n'.format(instrument.address)
    text += '## Timeout:        {:0.3f} s                                   ##\n'.format(instrument.serial.timeout)
    text += '###############################################################\n'
    minimalmodbus._print_out(text)
    instrument.debug = False
    
#    AQSX01_Conf.getGreenAmberThreshold(instrument)    
#    AQSX01_Conf.getAmberRedThreshold(instrument)
#    AQSX01_Conf.getSlaveAddress(instrument)
#    AQSX01_Conf.getTemperatureOffset(instrument)    
#    AQSX01_Conf.getEsp8266ResetPeriod(instrument)
        
#    temperatureOffset = 280    
#    AQSX01_Conf.setTemperatureOffset(instrument, temperatureOffset)
#    if AQSX01_Conf.getTemperatureOffset(instrument) == temperatureOffset:
#        minimalmodbus._print_out('\ntemperatureOffset set correctly')
#    else :
#        minimalmodbus._print_out('\nerror writing the temperatureOffset')    
    
#    resetPeriod = 300#seconds
#    AQSX01_Conf.setEsp8266ResetPeriod(instrument, resetPeriod)
#    if AQSX01_Conf.getEsp8266ResetPeriod(instrument) == resetPeriod:
#        minimalmodbus._print_out('\nresetPeriod set correctly')
#    else :
#        minimalmodbus._print_out('\nerror writing the resetPeriod')
        
        
    try:
        while True:     

            Sht31.getTemperatureAndHumidity(instrument, Configuration.delayMessages)

            Co2.getCo2(instrument, Configuration.delayMessages)
            
            #Sgp30.getSgp30Parameters(instrument, Configuration.delayMessages)
            
            time.sleep(Configuration.pollingCycle)#program cycle
            
    except KeyboardInterrupt:
        instrument.serial.close()
        pass

if __name__ == '__main__':
    main()
