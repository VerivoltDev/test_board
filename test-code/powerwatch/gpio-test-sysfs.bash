#!/bin/bash

init() {
  # Inputs
  # CN2_2
  echo 461 >/sys/class/gpio/export
  echo in >/sys/class/gpio/gpio461/direction
  # CN2_3
  echo 467 >/sys/class/gpio/export
  echo in >/sys/class/gpio/gpio467/direction

  #Ouputs
  # CN2_0
  echo 466 >/sys/class/gpio/export
  echo out >/sys/class/gpio/gpio466/direction
  # CN2_1
  echo 458 >/sys/class/gpio/export
  echo out >/sys/class/gpio/gpio458/direction
  echo "config finished"
}

read() {
  sleep 1
  echo "==> INPUTS"
  echo "CN2_2"
  cat /sys/class/gpio/gpio461/value
  echo "CN2_3"
  cat /sys/class/gpio/gpio467/value
}

write() {
  echo "OUTPUTS ==>"
  echo "CN2_1 $1"
  echo $1 >/sys/class/gpio/gpio458/value
  echo "CN2_0 $2"
  echo $2 >/sys/class/gpio/gpio466/value
}

gpio_echo() {
  write 0 0
  read
  write 0 1
  read
  write 1 0
  read
  write 1 1
  read
}

loop() {
  echo "loop"
  i=$1
  while [ $i -gt 0 ]; do
    gpio_echo
    i=$((i - 1))
  done
}

help() {
  echo "gpio-test-sysfs.bash init"
  echo "gpio-test-sysfs.bash loop num"
  echo "gpio-test-sysfs.bash read"
  echo "gpio-test-sysfs.bash write cn2_1 cn2_0"
  echo "gpio-test-sysfs.bash write 1 1"

}

if [[ -v 1 ]]; then
  if
    [ "$1" = "loop" ]
  then
    loop $2
  elif [ "$1" = "init" ]; then
    init
  elif [ "$1" = "read" ]; then
    read
  elif [ "$1" = "write" ]; then
    write $2 $3
  fi
else
  help
fi
