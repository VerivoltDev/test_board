f?=main.py
p?=/dev/ttyACM0
i?=./ESP32_GENERIC_C3-20241129-v1.24.1.bin

help: ## Prints help for targets with comments
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
	@echo "Examples to use"
	@echo "make putAsMain f=file.py"

erase: ## Erase flash
	esptool.py --port $p erase_flash

flash-mpy: ## Flashing esp32 memory
	esptool.py --port $p --baud 460800 write_flash 0 $i

put: ## Put script in micropython
	ampy -p $p put $f

putAsMain: ## Put scritp as main.py in micropython
	cp $f main.py
	ampy -p $p put main.py

t: ## Serialport init
	picocom $p -b 115200
