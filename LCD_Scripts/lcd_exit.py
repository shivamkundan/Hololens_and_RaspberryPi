import datetime
import lcddriver
import time
import sys

display = lcddriver.lcd()
display.lcd_display_string("   System Off", 1)
time.sleep(0.7)
display.lcd_display_string("    ystem Off", 1)
time.sleep(0.1)
display.lcd_display_string("     stem Off", 1)
time.sleep(0.1)
display.lcd_display_string("      tem Off", 1)
time.sleep(0.1)
display.lcd_display_string("       em Off", 1)
time.sleep(0.1)
display.lcd_display_string("        m Off", 1)
time.sleep(0.1)
display.lcd_display_string("          Off", 1)
time.sleep(0.1)
display.lcd_display_string("           ff", 1)
time.sleep(0.1)
display.lcd_display_string("            f", 1)
time.sleep(0.1)
display.lcd_display_string("             ", 1)
time.sleep(0.1)
display.lcd_display_string("Goodbye", 2)
time.sleep(1)
display.lcd_display_string("                ", 2)