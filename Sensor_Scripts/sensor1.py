import RPi.GPIO as GPIO
from time import sleep,time

GPIO.setmode(GPIO.BCM)

inpin=24
outpin=23

GPIO.setup(inpin,GPIO.IN)
GPIO.setup(outpin, GPIO.OUT)

while(1):
    GPIO.output(outpin,False)
    sleep(0.01)
    GPIO.output(outpin,True)
    sleep(0.01)
    GPIO.output(outpin,False)
    
    while(GPIO.input(inpin)==False):
          pass
    starttime=time()
    
    while(GPIO.input(inpin)==True):
        pass
    stoptime=time()
    
    total=float(stoptime-starttime)
    dist=100*(343*total)/2
    print('Distance: ',dist,'cm')
    sleep(1)
    
GPIO.cleanup()