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

from machine import Pin
from machine import UART
from time import sleep

uart1 = UART(1, baudrate=9600, tx=21, rx=20)

DE = Pin(9, Pin.OUT)
RE = Pin(10, Pin.OUT)

def send(msg, time=0.01):
    DE.value(1)
    RE.value(1)
    sleep(time)
    uart1.write(msg)

def receive(num, time=0.01):
    DE.value(0)
    RE.value(0)
    sleep(time)
    return uart1.read(num)

def start():
    while True:
        print("send")
        send("hello")
        sleep(0.5)
