th_logger: th_logger.c
	gcc th_logger.c -o th_logger -lwiringPi -lwiringPiDev

clean: 
	rm -f *.o th_logger
