import datetime
import lcddriver
import time
import sys
import shlex

display = lcddriver.lcd()
s=str(sys.argv).split(' ')
s1=' '.join(sys.argv[1:3])
s2=' '.join(sys.argv[3:])
display.lcd_display_string("                ", 1)
display.lcd_display_string("                ", 2)
display.lcd_display_string(s1, 1)
display.lcd_display_string(s2, 2)
