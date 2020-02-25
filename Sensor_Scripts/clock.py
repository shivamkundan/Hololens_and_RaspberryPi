# Shivam Kundan
# S17-71-HOL1
# Fall 2017

# Constantly receive data from sensors, display on LCD, and save to file

# TSL2561 Light Sensor
# SI7021 Temperature & Humidity Sensor

import smbus
import time
import datetime
from subprocess import call
import sys
import shlex
import lcddriver

# Get I2C bus
bus = smbus.SMBus(1)

# Initialize LCD
display = lcddriver.lcd()

def brightness_sensor():
	# TSL2561 address, 0x39(57)
	# Select control register, 0x00(00) with command register, 0x80(128)
	#		0x03(03)	Power ON mode
	bus.write_byte_data(0x39, 0x00 | 0x80, 0x03)
	# TSL2561 address, 0x39(57)
	# Select timing register, 0x01(01) with command register, 0x80(128)
	#		0x02(02)	Nominal integration time = 402ms
	bus.write_byte_data(0x39, 0x01 | 0x80, 0x02)

	time.sleep(0.5)

	# Read data back from 0x0C(12) with command register, 0x80(128), 2 bytes
	# ch0 LSB, ch0 MSB
	data = bus.read_i2c_block_data(0x39, 0x0C | 0x80, 2)

	# Read data back from 0x0E(14) with command register, 0x80(128), 2 bytes
	# ch1 LSB, ch1 MSB
	data1 = bus.read_i2c_block_data(0x39, 0x0E | 0x80, 2)

	# Convert the data
	ch0 = data[1] * 256 + data[0]
	ch1 = data1[1] * 256 + data1[0]
	brightness = ch0-ch1

	# Output data to screen
	#print ("Full Spectrum(IR + Visible) :%d lux" %ch0)
	#print ("Infrared Value :%d lux" %ch1)
	#print ("Visible Value :%d lux" %brightness)
	return brightness
	
def humidity_sensor():
	# SI7021 address, 0x40(64)
	#		0xF5(245)	Select Relative Humidity NO HOLD master mode
	bus.write_byte(0x40, 0xF5)

	time.sleep(0.3)

	# SI7021 address, 0x40(64)
	# Read data back, 2 bytes, Humidity MSB first
	data0 = bus.read_byte(0x40)
	data1 = bus.read_byte(0x40)

	# Convert the data
	humidity = float(((data0 * 256 + data1) * 125 / 65536.0) - 6)
	lcdHumidity=str(round(humidity)).split('.')[0]
	a,b=str(humidity).split('.')
	logHumidity=float('.'.join((a, b[0:3])))
	#print ("Relative Humidity: %.2f %%" %logHumidity)
	return (lcdHumidity,logHumidity) 

def temperature_sensor():
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
	lcdTemp=float('.'.join((a, b[0:1])))
	logTemp=float('.'.join((a, b[0:3])))
	fTemp = cTemp * 1.8 + 32

	# Output data to screen	
	#print ("Temperature in Celsius is : %.2f C" %cTemp)
	#print ("Temperature in Fahrenheit is : %.2f F" %fTemp)

	return (lcdTemp,logTemp)

def get_date_time():
	now = datetime.datetime.now()
	myDate=now.strftime('%a,%b%-d')		#Short value because of 16-char lcd
	myTime=now.strftime('%-I:%M%p')
	logDate=now.strftime('%m/%d/%y')	#The logged date is in a different format than lcd's date
	logTime=now.strftime('%-I:%M:%S%p')
	return (myDate,myTime,logDate,logTime)

try:
	display.lcd_display_string("                ", 1)
	display.lcd_display_string("                ", 2)
	num_writes=0
	flag=0
	b_s_last=0
	while True:
		if (flag!=0):
                    b_s_last=b_s

		#Get current values
		myDate,myTime,logDate,logTime=get_date_time()
		lcdTemp,logTemp=temperature_sensor()
		lcdHumidity,logHumidity=humidity_sensor()
		b_s=brightness_sensor()
		flag=1
		print(b_s_last)
		print(b_s)
		
		#Automated night light 'smart service'
		if (abs(b_s-b_s_last)>300):
                    print("("+myDate+" "+str(logTime)+")"+"\n"+str(logTemp)+"C "+str(logHumidity)+"% "+str(b_s)+"lux "+"\n")
                    if (b_s<100):
                        call(["omxplayer","-o","local","/home/pi/seniordesign/Audio_Files/Speech/main_lights_off.mp3"])
                        call(["gpio","-g", "write", "27", "1"])
                    elif (b_s>100 and b_s<500):
                        call(["omxplayer","-o","local","/home/pi/seniordesign/Audio_Files/Speech/dim_lights.mp3"])
                        call(["gpio","-g", "write", "27", "0"])
                    if (b_s>500):
                        call(["omxplayer","-o","local","/home/pi/seniordesign/Audio_Files/Speech/main_lights_on.mp3"])
                        call(["gpio","-g", "write", "27", "0"])
                                       
		#Print to lcd
		l1=myDate+" "+myTime
		display.lcd_display_string(l1, 1)
		l2=str(lcdTemp)+"C "+str(lcdHumidity)+"% "+str(b_s)+"Lux"
		display.lcd_display_string("                ", 2)
		display.lcd_display_string(l2, 2)

		#Write to log file
		f=open('lab_sensors_log.csv','a')
		outstring = logDate+","+str(logTime)+','+str(logTemp)+"C"+','+str(logHumidity)+"%"+','+str(b_s)+"lux"+"\n"
		f.write(outstring)
		num_writes+=1
		f.close()
		print("#writes: " + str(num_writes))
				
		time.sleep(2) #Makes a total of 4 reading/minute

except KeyboardInterrupt:
    #print("Cleaning up!")
    display.lcd_clear()
    s="#writes: " + str(num_writes)
    display.lcd_display_string(s, 2)
