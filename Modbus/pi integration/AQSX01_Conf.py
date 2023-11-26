# AQS-X01 @ BytesOnTheRocks

import minimalmodbus
import ModbusRegistersConf


def getGreenAmberThreshold(instrument: minimalmodbus.Instrument) -> int:
    minimalmodbus._print_out("\nGreenAmberThreshold:")
    greenAmberThreshold = instrument.read_register(
        ModbusRegistersConf.GREENAMBER_THRESHOLD_REG_NUM, 0, 3, False
    )
    minimalmodbus._print_out(repr(greenAmberThreshold))  # FC 3: Read Holding Register
    return int(greenAmberThreshold)


def setGreenAmberThreshold(
    instrument: minimalmodbus.Instrument, valueToWrite: int
) -> None:
    instrument.write_register(
        ModbusRegistersConf.GREENAMBER_THRESHOLD_REG_NUM, valueToWrite, 0, 6, False
    )


def getAmberRedThreshold(instrument: minimalmodbus.Instrument) -> int:
    minimalmodbus._print_out("\nAmberRedThreshold:")
    amberRedThreshold = instrument.read_register(
        ModbusRegistersConf.AMBERRED_THRESHOLD_REG_NUM, 0, 3, False
    )
    minimalmodbus._print_out(repr(amberRedThreshold))  # FC 3: Read Holding Register
    return int(amberRedThreshold)


def setAmberRedThreshold(
    instrument: minimalmodbus.Instrument, valueToWrite: int
) -> None:
    instrument.write_register(
        ModbusRegistersConf.AMBERRED_THRESHOLD_REG_NUM, valueToWrite, 0, 6, False
    )


def getSlaveAddress(instrument: minimalmodbus.Instrument) -> int:
    minimalmodbus._print_out("\nSlaveAddress:")
    slaveAddress = instrument.read_register(
        ModbusRegistersConf.SLAVE_ADDRESS_REG_NUM, 0, 3, False
    )
    minimalmodbus._print_out(repr(slaveAddress))  # FC 3: Read Holding Register
    return int(slaveAddress)


def setSlaveAddress(instrument: minimalmodbus.Instrument, valueToWrite: int) -> None:
    instrument.write_register(
        ModbusRegistersConf.SLAVE_ADDRESS_REG_NUM, valueToWrite, 0, 6, False
    )


def getTemperatureOffset(instrument: minimalmodbus.Instrument) -> int:
    minimalmodbus._print_out("\nTemperatureOffset:")
    temperatureOffset = instrument.read_register(
        ModbusRegistersConf.SHT31_TEMPERATURE_OFFSET_REG_NUM, 0, 3, False
    )
    minimalmodbus._print_out(repr(temperatureOffset))  # FC 3: Read Holding Register
    return int(temperatureOffset)


def setTemperatureOffset(
    instrument: minimalmodbus.Instrument, valueToWrite: int
) -> None:
    instrument.write_register(
        ModbusRegistersConf.SHT31_TEMPERATURE_OFFSET_REG_NUM, valueToWrite, 0, 6, False
    )


def getEsp8266ResetPeriod(instrument: minimalmodbus.Instrument) -> int:
    minimalmodbus._print_out("\nEsp8266ResetPeriod:")
    esp8266ResetPeriod = instrument.read_register(
        ModbusRegistersConf.ESP8266_HOLD_RESET_PERIOD_REG_NUM, 0, 3, False
    )
    minimalmodbus._print_out(repr(esp8266ResetPeriod))  # FC 3: Read Holding Register
    return int(esp8266ResetPeriod)


def setEsp8266ResetPeriod(
    instrument: minimalmodbus.Instrument, valueToWrite: int
) -> None:
    instrument.write_register(
        ModbusRegistersConf.ESP8266_HOLD_RESET_PERIOD_REG_NUM, valueToWrite, 0, 6, False
    )
