import datetime
import lcddriver
import time
import sys

display = lcddriver.lcd()
s1=' '.join(sys.argv[1:])
display.lcd_display_string("                ", 2)
display.lcd_display_string(str(s1), 2)




