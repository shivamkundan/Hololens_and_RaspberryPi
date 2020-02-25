import sys
import lcddriver

display = lcddriver.lcd()
display.lcd_display_string("                ", 1)
s1="received:"+sys.argv[1]
display.lcd_display_string(s1, 1)