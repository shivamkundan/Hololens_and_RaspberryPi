import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
PWR=17
#GPIO.setwarnings(False)
GPIO.setup(PWR,GPIO.OUT)
GPIO.output(PWR,True)
time.sleep(5)
GPIO.output(PWR,False)
GPIO.cleanup()