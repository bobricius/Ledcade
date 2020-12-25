
avrdude -C ./avrdude.conf -p m328p -i 5000000000000 -c usbasp -P usb -U lfuse:w:0xE2:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m -U lock:w:0xF3:m

pause	
