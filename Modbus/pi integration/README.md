# AQS-X01 wired communication

### Overview:

There are 2 ways to integrate the AQS-X01 to a Raspberry Pi (or similar): Wifi (e.g. MQTT) or Modbus RTU.

Easy way to support Modbus RTU in a Raspberry Pi, Linux or Windows machine: USB to 485 cable + Python.

Additional Python Modules required:

- pip install pyserial
- pip install minimalmodbus

Get the scripts in this folder. Edit Configuration.py with you usb port '/dev/ttyUSBX' and execute main.py.

You are ready to integrate the read data into your application.

## Using poetry
poetry install
poetry run python main.py

## Running mypy
Note: --strict flag is the strictest it can be but working with third party libraries may require to be more flexible

poetry run mypy . --strict --ignore-missing-imports

## Running Ruff
poetry run ruff check .
