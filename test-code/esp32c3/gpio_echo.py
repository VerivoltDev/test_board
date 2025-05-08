'''
Brief: Its only echo script. Note: Change Pin number
'''
from machine import Pin
from time import sleep

conn0 = Pin(8, Pin.IN) # Read
conn1 = Pin(7, Pin.IN) # Read
echo0 = Pin(5, Pin.OUT) # Write
echo1 = Pin(6, Pin.OUT) # Write

while True:
    echo0.value(1) if conn0.value() else echo0.value(0)
    echo1.value(1) if conn1.value() else echo1.value(0)
