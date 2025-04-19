from machine import Pin
from time import sleep

conn0 = Pin(8, Pin.IN) # Read
conn1 = Pin(6, Pin.IN) # Read
conn2 = Pin(5, Pin.OUT) # Write
conn3 = Pin(7, Pin.OUT) # Write

while True:
    conn3.value(1) if conn0.value() else conn3.value(0)
    conn2.value(1) if conn1.value() else conn2.value(0)
