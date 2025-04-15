erase:
	esptool.py --port /dev/ttyACM0 erase_flash

flash-mpy:
	esptool.py --port /dev/ttyACM0 --baud 460800 write_flash 0 ./ESP32_GENERIC_C3-20241129-v1.24.1.bin

put:
	ampy -p /dev/ttyACM0 put main.py

t:
	picocom /dev/ttyACM0 -b 115200
