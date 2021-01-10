#AQS-X01 @ BytesOnTheRocks

import time
import minimalmodbus
import ModbusRegisters

def getSgp30Parameters(instrument, delay):

        time.sleep(delay)        
        minimalmodbus._print_out('\nTVOC(ppb):')
        minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.TVOC_REG_NUM, 0, 4, False)))#FC 4: Read Register

        time.sleep(delay)        
        minimalmodbus._print_out('\neCO2(ppm):')
        minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.ECO2_REG_NUM, 0, 4, False)))#FC 4: Read Register

        time.sleep(delay)        
        minimalmodbus._print_out('\nTVOC Average(ppb):')
        minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.TVOC_AVG_REG_NUM, 0, 4, False)))#FC 4: Read Register

        time.sleep(delay)        
        minimalmodbus._print_out('\neCO2 Average(ppm):')
        minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.ECO2_AVG_REG_NUM, 0, 4, False)))#FC 4: Read Register

        time.sleep(delay)        
        minimalmodbus._print_out('\nTVOC base:')
        minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.TVOC_BASE_REG_NUM, 0, 4, False)))#FC 4: Read Register

        time.sleep(delay)        
        minimalmodbus._print_out('\neCO2 base:')
        minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.ECO2_BASE_REG_NUM, 0, 4, False)))#FC 4: Read Register
            
        time.sleep(delay)        
        minimalmodbus._print_out('\nEthanol raw (ppm):')
        minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.ETHANOL_BASE_REG_NUM, 0, 4, False)))#FC 4: Read Register

        time.sleep(delay)        
        minimalmodbus._print_out('\nH2 raw (ppm):')
        minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.H2RAW_BASE_REG_NUM, 0, 4, False)))#FC 4: Read Register
