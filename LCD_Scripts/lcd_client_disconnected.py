import lcddriver
import sys
import time

display = lcddriver.lcd()
line2="#Clients:"+str(sys.argv[1])
display.lcd_display_string(line2, 2)
display.lcd_display_string("Client Disconnec", 1)
time.sleep(0.2)
display.lcd_display_string("lient Disconnect", 1)
time.sleep(0.2)
display.lcd_display_string("ient Disconnecte", 1)
time.sleep(0.2)
display.lcd_display_string("ent Disconnected", 1)



