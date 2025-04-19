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

uart1 = UART(1, baudrate=9600, tx=20, rx=21)

DE = Pin(10, Pin.OUT)
RE = Pin(9, Pin.OUT)

def on():
    DE.value(1)
    RE.value(0)

def off():
    DE.value(0)
    RE.value(1)


def send(msg, time=0.01):
    sleep(time)
    uart1.write(msg)

def receive(num, time=0.01):
    sleep(time)
    return uart1.read(num)

def sendloop():
    i = 0
    while True:
        i += 1
        send("hello")
        print(i)
        sleep(0.5)

def receivedloop():
    i = 0
    while True:
        i += 1
        print(receive(5))
        print(i)
        sleep(0.5)

on()
