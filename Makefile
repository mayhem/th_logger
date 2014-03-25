<<<<<<< HEAD
poll_and_log_sht15: logSHT1x.c RPi_SHT1x.c  RPi_SHT1x.h
	gcc -I ../bcm2835/src -lm -o poll_and_log_sht15 ./../bcm2835/src/bcm2835.c ./RPi_SHT1x.c logSHT1x.c
=======
th_logger: th_logger.c
	gcc th_logger.c -o th_logger -lwiringPi -lwiringPiDev

clean: 
	rm -f *.o th_logger
>>>>>>> e388a03641cc521f24c11eed2d40af6246186ee1
