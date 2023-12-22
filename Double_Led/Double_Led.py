#!/usr/bin/env python3
import Jetson.GPIO as GPIO
import time
 
pin_R = 11 
pin_G = 12 
 
GPIO.setmode(GPIO.BOARD)       
GPIO.setwarnings(False)        
GPIO.setup(pin_R,GPIO.OUT)
GPIO.setup(pin_G,GPIO.OUT)
 
 
def setColor():
    GPIO.output(pin_R,GPIO.HIGH)
    GPIO.output(pin_G,GPIO.LOW)
    time.sleep(5)
    GPIO.output(pin_R,GPIO.LOW)
    GPIO.output(pin_G,GPIO.HIGH)
    time.sleep(5)
    
def loop():
    while True:
        setColor()
        
def destroy():
    GPIO.output(pin_R,GPIO.LOW)
    GPIO.output(pin_G,GPIO.LOW)
    GPIO.cleanup()
 
 
if __name__ == "__main__":
    try:
        loop()
    except KeyboardInterrupt:
        destroy()