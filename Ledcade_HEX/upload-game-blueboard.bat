avrdude -C ./avrdude.conf -F -v -p m328p -c usbasp -P usb -e

avrdude -C ./avrdude.conf -p m328p -c usbasp -P usb -U  lfuse:w:0xE2:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m -U lock:w:0xF3:m
	
avrdude -C ./avrdude.conf -F -v -p m328p -c usbasp -P usb  -i 100  -Uflash:w:game.hex:i

pause
