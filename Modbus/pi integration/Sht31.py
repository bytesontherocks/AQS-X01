# AQS-X01 @ BytesOnTheRocks

import time
import minimalmodbus  
import ModbusRegisters


def getTemperatureAndHumidity(instrument: minimalmodbus.Instrument, delay: float) -> None:
    time.sleep(delay)
    minimalmodbus._print_out("\nTemperature (degC):")
    minimalmodbus._print_out(
        repr(instrument.read_register(ModbusRegisters.TEMP_REG_NUM, 2, 4, True))
    )  # FC 4: Read Register

    time.sleep(delay)
    minimalmodbus._print_out("\nHumidity (%):")
    minimalmodbus._print_out(
        repr(instrument.read_register(ModbusRegisters.HUM_REG_NUM, 2, 4, False))
    )  # FC 4: Read Register
