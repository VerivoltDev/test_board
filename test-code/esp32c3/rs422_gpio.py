'''
chip  -- esp32 -- CONN
RO  1 -- 20
RE  2 -- 10
DE  3 --  9
DI  4 -- 21
GND 5
A   6         -- 14
B   7         -- 13
VCC 8

    '''

'''
module  esp32   conn    description
tx      21
rx      20
re      10
de      9
'''

from machine import Pin
from machine import UART
from time import sleep

uart1 = UART(1, baudrate=9600, tx=9, rx=20, timeout=1000)

DE = Pin(10, Pin.OUT)
RE = Pin(21, Pin.OUT)

def gpio_on():
    DE.value(1)
    RE.value(0)

def gpio_off():
    DE.value(0)
    RE.value(1)

# Def pins
conn0 = Pin(8, Pin.IN) # Read
conn1 = Pin(7, Pin.IN) # Read
echo0 = Pin(5, Pin.OUT) # Write
echo1 = Pin(6, Pin.OUT) # Write

# Def callback
def cb_echo(pin):
    echo0.value(1) if conn0.value() else echo0.value(0)
    echo1.value(1) if conn1.value() else echo1.value(0)


try:
    conn0.irq(trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING, handler=cb_echo)
    conn1.irq(trigger=Pin.IRQ_FALLING | Pin.IRQ_RISING, handler=cb_echo)
except OSError as e:
    print(e)


gpio_on()

while True:
    data = uart1.readline()
    # data = uart1.read(9)
    if data == None:
        print("Empty")
    else:
        uart1.write(data)
        print(data)

