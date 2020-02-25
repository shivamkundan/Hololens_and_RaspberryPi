# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# SI7021
# This code is designed to work with the SI7021_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Humidity?sku=SI7021_I2CS#tabs-0-product_tabset-2
import smbus
import time
import datetime
from subprocess import call

# Get I2C bus
bus1 = smbus.SMBus(1)

# SI7021 address, 0x40(64)
#		0xF5(245)	Select Relative Humidity NO HOLD master mode
bus1.write_byte(0x40, 0xF5)

time.sleep(0.3)

# SI7021 address, 0x40(64)
# Read data back, 2 bytes, Humidity MSB first
data0 = bus1.read_byte(0x40)
data1 = bus1.read_byte(0x40)

# Convert the data
humidity = ((data0 * 256 + data1) * 125 / 65536.0) - 6
a,b=str(humidity).split('.')
humidity=float('.'.join((a, b[0:2])))

time.sleep(0.3)

# SI7021 address, 0x40(64)
#		0xF3(243)	Select temperature NO HOLD master mode
bus1.write_byte(0x40, 0xF3)

time.sleep(0.3)

# SI7021 address, 0x40(64)
# Read data back, 2 bytes, Temperature MSB first
data0 = bus1.read_byte(0x40)
data1 = bus1.read_byte(0x40)

# Convert the data
cTemp = ((data0 * 256 + data1) * 175.72 / 65536.0) - 46.85
a,b=str(cTemp).split('.')
cTemp=float('.'.join((a, b[0:2])))
fTemp = cTemp * 1.8 + 32

# Output data to screen
print ("Relative Humidity is : %.2f %%" %humidity)
print ("Temperature in Celsius is : %.2f C" %cTemp)
print ("Temperature in Fahrenheit is : %.2f F" %fTemp)

now = datetime.datetime.now()
date=now.strftime('%m/%d/%y')
hour=now.strftime('%H:%M:%S')
s1="python"
s2="/home/pi/seniordesign/LCD_Scripts/lcd_print.py"
s3=str(date)
lcd_hour=now.strftime('%H:%M')
s4=str(lcd_hour)
s5=str(cTemp)+"C"
s6=str(humidity)+"%"
call([s1,s2,s3,s4,s5,s6])