'''
Brief: Its only echo script. Note: Change Pin number
'''
from machine import Pin
from time import sleep

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


# while True:
#     sleep(3)
#     print("hola")
