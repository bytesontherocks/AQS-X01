#AQS-X01 @ BytesOnTheRocks

import minimalmodbus  # type: ignore
import ModbusRegistersConf

def getGreenAmberThreshold(instrument) -> int:       
	minimalmodbus._print_out('\nGreenAmberThreshold:')
	greenAmberThreshold = instrument.read_register(ModbusRegistersConf.GREENAMBER_THRESHOLD_REG_NUM, 0, 3, False)
	minimalmodbus._print_out(repr(greenAmberThreshold))#FC 3: Read Holding Register
	return greenAmberThreshold

def setGreenAmberThreshold(instrument, valueToWrite) -> None:     
	instrument.write_register(ModbusRegistersConf.GREENAMBER_THRESHOLD_REG_NUM, valueToWrite, 0, 6, False)

def getAmberRedThreshold(instrument) -> int:      
	minimalmodbus._print_out('\nAmberRedThreshold:')
	amberRedThreshold = instrument.read_register(ModbusRegistersConf.AMBERRED_THRESHOLD_REG_NUM, 0, 3, False)
	minimalmodbus._print_out(repr(amberRedThreshold))#FC 3: Read Holding Register
	return amberRedThreshold

def setAmberRedThreshold(instrument, valueToWrite : int) -> None:        
	instrument.write_register(ModbusRegistersConf.AMBERRED_THRESHOLD_REG_NUM, valueToWrite, 0, 6, False)
     
def getSlaveAddress(instrument) -> int:      
	minimalmodbus._print_out('\nSlaveAddress:')
	slaveAddress = instrument.read_register(ModbusRegistersConf.SLAVE_ADDRESS_REG_NUM, 0, 3, False)
	minimalmodbus._print_out(repr(slaveAddress))#FC 3: Read Holding Register
	return slaveAddress

def setSlaveAddress(instrument, valueToWrite : int) -> None:       
	instrument.write_register(ModbusRegistersConf.SLAVE_ADDRESS_REG_NUM, valueToWrite, 0, 6, False)

def getTemperatureOffset(instrument) -> int:      
	minimalmodbus._print_out('\nTemperatureOffset:')
	temperatureOffset = instrument.read_register(ModbusRegistersConf.SHT31_TEMPERATURE_OFFSET_REG_NUM, 0, 3, False)
	minimalmodbus._print_out(repr(temperatureOffset))#FC 3: Read Holding Register
	return temperatureOffset

def setTemperatureOffset(instrument, valueToWrite : int) -> None:      
	instrument.write_register(ModbusRegistersConf.SHT31_TEMPERATURE_OFFSET_REG_NUM, valueToWrite, 0, 6, False)

def getEsp8266ResetPeriod(instrument) -> int:      
	minimalmodbus._print_out('\nEsp8266ResetPeriod:')
	esp8266ResetPeriod = instrument.read_register(ModbusRegistersConf.ESP8266_HOLD_RESET_PERIOD_REG_NUM, 0, 3, False)
	minimalmodbus._print_out(repr(esp8266ResetPeriod))#FC 3: Read Holding Register
	return esp8266ResetPeriod

def setEsp8266ResetPeriod(instrument, valueToWrite : int) -> None:       
	instrument.write_register(ModbusRegistersConf.ESP8266_HOLD_RESET_PERIOD_REG_NUM, valueToWrite, 0, 6, False)