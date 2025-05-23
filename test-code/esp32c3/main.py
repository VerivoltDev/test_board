'''
Brief: Its only echo script. Note: Change Pin number
'''
from machine import Pin

# Def pins
conn0 = Pin(8, Pin.IN) # Read
conn1 = Pin(7, Pin.IN) # Read
echo0 = Pin(5, Pin.OUT) # Write
echo1 = Pin(6, Pin.OUT) # Write

# Def callback
def cb_echo():
    echo0.value(1) if conn0.value() else echo0.value(0)
    echo1.value(1) if conn1.value() else echo1.value(0)


conn0.irq(trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING, handler=cb_echo)
conn1.irq(trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING, handler=cb_echo)

from machine import sleep

while True:
    sleep(1)
