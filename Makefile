poll_and_log_sht15: logSHT1x.c RPi_SHT1x.c  RPi_SHT1x.h
	gcc -I ../bcm2835/src -lm -o poll_and_log_sht15 ./../bcm2835/src/bcm2835.c ./RPi_SHT1x.c logSHT1x.c
