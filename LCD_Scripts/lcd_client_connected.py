import lcddriver
import sys

display = lcddriver.lcd()
display.lcd_display_string("Client Connected", 1)
line2="#Clients:"+str(sys.argv[1])
display.lcd_display_string(line2, 2)
