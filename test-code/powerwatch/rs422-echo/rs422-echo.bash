#!/bin/bash

init() {
  # Config GPIO0_60
  devmem2 0x000F40F4 w 0x00050007
  echo 486 >/sys/class/gpio/export
  echo out >/sys/class/gpio/gpio486/direction
  # Config GPIO0_61
  devmem2 0x000F40F8 w 0x00050007
  echo 485 >/sys/class/gpio/export
  echo out >/sys/class/gpio/gpio485/direction
}

set() {
  echo 0 >/sys/class/gpio/gpio486/value
  echo 0 >/sys/class/gpio/gpio485/value
}
