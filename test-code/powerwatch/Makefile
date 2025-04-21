p?=/dev/ttyUSB0
m?=/dev/ttyUSB1
f?=namefileToSend

help: ## Prints help for targets with comments
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

t: ## serialport linux connect
	minicom -o -b 115200 -D $p

spi: ## serialport spi connect
	minicom -o -b 115200 -D $m

sendFile: ## Enviar archivos por minicom
	@echo "Debe configurar el destino con minicom, minicom detecta automáticamente el zmode"
	sz $f -r --zmode > $p < $p
