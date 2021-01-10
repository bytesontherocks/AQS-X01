#AQS-X01 @ BytesOnTheRocks

import time
import minimalmodbus
import ModbusRegisters

def getCo2(instrument, delay):

	time.sleep(delay)        
	minimalmodbus._print_out('\nCO2(ppm):')
	minimalmodbus._print_out(repr(instrument.read_register(ModbusRegisters.CO2_REG_NUM, 0, 4, False)))#FC 4: Read Register

     
