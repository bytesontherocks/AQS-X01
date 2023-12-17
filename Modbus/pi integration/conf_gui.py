import tkinter as tk  # It is used to make Graphical User Interface.
import serial
import threading  # Threading in python is used to run multiple task or functions at the same time.
import time
# Idea from:
# https://github.com/deepanshu13-eng/LIVE-SENSOR-UPDATES-ON-GRAPHICAL-USER-INTERFACE-USING-PYTHON-AND-ARDUINO/blob/main/sensorgui.py

serialPort = serial.Serial(
    port="COM5", baudrate=115000, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE
)
serialString = ""  # Used to hold data coming over UART

root = tk.Tk()  # The root window is created. The root window is a main application window in our program.
root.title("AQS-X01")  # Giving title to our Graphical User Interface.
root.geometry("400x200")  # Setting up Geometry of our Graphical User Interface.
root.configure()  # Setting up the Background Colour of our Graphical User Interface.


def readModbusData() -> None:
    while True:
        time.sleep(5)
        threshold_green_amber.set(AQSX01_Conf.getGreenAmberThreshold(instrument))
        threshold_amber_red.set(AQSX01_Conf.getAmberRedThreshold(instrument))
        modbus_slave_address.set(AQSX01_Conf.getSlaveAddress(instrument))
        temperature_offset.set(AQSX01_Conf.getTemperatureOffset(instrument))
        wifi_reset_period.set(AQSX01_Conf.getEsp8266ResetPeriod(instrument))


            # # Read data out of the buffer until a carriage return / new line is found
            # serial_string = serialPort.readline().decode("Ascii").strip()

            # if serial_string.startswith("CO2:"):
            #
            # elif serial_string.startswith("CO2 average:"):
            #     co2_average.set(serial_string.split(":")[1])
            # elif serial_string.startswith("temperature:"):
            #     temperature.set(serial_string.split(":")[1])
            # elif serial_string.startswith("humidity:"):
            #     humidity.set(serial_string.split(":")[1])


tk.Label(root)  # Defining a label
tk.Label(
    root, text="Sensor data (5s update)", font=("Lato", 20, "bold"), bg="light grey"
).grid(
    row=0, column=0, sticky="w"
)  # Setting up Font size, Colour ,row ,Font type, Row and Column of a Label.

tk.Label(root, text="threshold_green_amber:", font=("Lato", 20, "bold")).grid(row=1, column=0, sticky="w")
threshold_green_amber = (
    tk.StringVar()
)
tk.Label(root, textvariable=threshold_green_amber, font=("Lato", 20, "bold")).grid(
    row=1, column=1, sticky="w"
)

tk.Label(root, text="threshold_green_amber:", font=("Lato", 20, "bold")).grid(
    row=2, column=0, sticky="w"
)
threshold_green_amber = (
    tk.StringVar()
)
tk.Label(root, textvariable=threshold_green_amber, font=("Lato", 20, "bold")).grid(
    row=2, column=1, sticky="w"
)

tk.Label(root, text="Temperature:", font=("Lato", 20, "bold")).grid(
    row=3, column=0, sticky="w"
)
temperature = (
    tk.StringVar()
)
tk.Label(root, textvariable=temperature, font=("Lato", 20, "bold")).grid(
    row=3, column=1, sticky="w"
)

tk.Label(root, text="Humidity:", font=("Lato", 20, "bold")).grid(
    row=4, column=0, sticky="w"
)
humidity = (
    tk.StringVar()
)
tk.Label(root, textvariable=humidity, font=("Lato", 20, "bold")).grid(
    row=4, column=1, sticky="w"
)

threading.Thread(
    target=readModbusData, daemon=True
).start()

root.mainloop()
