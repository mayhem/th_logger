poll_and_log_sht15: th_logger.c RPi_SHT1x.c  RPi_SHT1x.h
	gcc -I ../bcm2835/src -lm -o th_logger ./../bcm2835/src/bcm2835.c ./RPi_SHT1x.c th_logger.c

clean: 
	rm -f *.o th_logger
