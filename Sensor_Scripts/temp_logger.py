# Raspberry Pi Temperature Logger
# Zagros Robotics, Inc.
# www.zagrosrobotics.com
# 6/25/2013

import smbus
import time
import datetime
from subprocess import call

#SMBus(0) - Raspberry Pi Model A
#SMBus(1) - Raspberry Pi Model B

bus = smbus.SMBus(1)

#I2C address of sensor
address = 0x40

def temperature():
    time.sleep(0.3)

    # SI7021 address, 0x40(64)
    #		0xF3(243)	Select temperature NO HOLD master mode
    bus.write_byte(0x40, 0xF3)

    time.sleep(0.3)

    # SI7021 address, 0x40(64)
    # Read data back, 2 bytes, Temperature MSB first
    data0 = bus.read_byte(0x40)
    data1 = bus.read_byte(0x40)

    # Convert the data
    cTemp = ((data0 * 256 + data1) * 175.72 / 65536.0) - 46.85
    a,b=str(cTemp).split('.')
    cTemp=float('.'.join((a, b[0:3])))
    #fTemp = cTemp * 1.8 + 32
    return  cTemp

def humidity():
    # SI7021 address, 0x40(64)
    #		0xF5(245)	Select Relative Humidity NO HOLD master mode
    bus.write_byte(0x40, 0xF5)

    time.sleep(0.3)

    # SI7021 address, 0x40(64)
    # Read data back, 2 bytes, Humidity MSB first
    data0 = bus.read_byte(0x40)
    data1 = bus.read_byte(0x40)

    # Convert the data
    humidity = ((data0 * 256 + data1) * 125 / 65536.0) - 6
    a,b=str(humidity).split('.')
    humidity=float('.'.join((a, b[0:3])))
    return humidity

print("Temperature Data Logger\n")

try:

    while True:
        
        #Open Log File
        f=open('temperature_log.csv','a')
        now = datetime.datetime.now()
        date=now.strftime('%m/%d/%y')
        hour=now.strftime('%H:%M:%S')
        humid=humidity()
        cTemp = temperature()
        outstring = str(date)+","+str(hour)+','+str(cTemp)+" C"+','+str(humid)+" %"+"\n"
        print (outstring)
        s1="python"
        s2="/home/pi/seniordesign/LCD_Scripts/lcd_print.py"
        s3=str(date)
        lcd_hour=now.strftime('%H:%M')
        s4=str(lcd_hour)
        s5=str(cTemp)+"C"
        s6=str(humid)+"%"
        call([s1,s2,s3,s4,s5,s6])
        f.write(outstring)
        f.close()

        #log temperature every 61 seconds
        time.sleep(2)
    
finally:
    call(["python","/home/pi/seniordesign/LCD_Scripts/lcd_clear.py"])