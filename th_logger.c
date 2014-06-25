// Compile with: gcc -lm -o testSHT1x ./../bcm2835-1.8/src/bcm2835.c ./RPi_SHT1x.c testSHT1x.c

/*
Raspberry Pi SHT1x communication library.
By:      John Burns (www.john.geek.nz)
         Robert Kaye (mayhem-chaos.net)
Date:    01 November 2012
License: CC BY-SA v3.0 - http://creativecommons.org/licenses/by-sa/3.0/

This is a derivative work based on
    Name: Nice Guy SHT11 library
    By: Daesung Kim
    Date: 04/04/2011
    Source: http://www.theniceguy.net/2722
    License: Unknown - Attempts have been made to contact the author

Dependencies:
    BCM2835 Raspberry Pi GPIO Library - http://www.open.com.au/mikem/bcm2835/

Sensor:
    Sensirion SHT11 Temperature and Humidity Sensor interfaced to Raspberry Pi GPIO port

SHT pins:
    1. GND  - Connected to GPIO Port P1-06 (Ground)
    2. DATA - Connected via a 10k pullup resistor to GPIO Port P1-01 (3V3 Power)
    2. DATA - Connected to GPIO Port P1-18 (GPIO 24)
    3. SCK  - Connected to GPIO Port P1-16 (GPIO 23)
    4. VDD  - Connected to GPIO Port P1-01 (3V3 Power)

Note:
    GPIO Pins can be changed in the Defines of RPi_SHT1x.h
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <bcm2835.h>
#include <stdio.h>
#include "RPi_SHT1x.h"

int readTempAndHumidity(float *temp, float *hum)
{
    unsigned char noError = 1;  
    value humi_val,temp_val;
    
    // Wait at least 11ms after power-up (chapter 3.1)
    delay(20); 
    
    // Set up the SHT1x Data and Clock Pins
    SHT1x_InitPins();
    
    // Reset the SHT1x
    SHT1x_Reset();
    
    // Request Temperature measurement
    noError = SHT1x_Measure_Start( SHT1xMeaT );
    if (!noError) {
        return 1;
    }
        
    // Read Temperature measurement
    noError = SHT1x_Get_Measure_Value( (unsigned short int*) &temp_val.i );
    if (!noError) {
        return 1;
    }

    // Request Humidity Measurement
    noError = SHT1x_Measure_Start( SHT1xMeaRh );
    if (!noError) {
        return 1;
    }
        
    // Read Humidity measurement
    noError = SHT1x_Get_Measure_Value( (unsigned short int*) &humi_val.i );
    if (!noError) {
        return 1;
    }

    // Convert intergers to float and calculate true values
    temp_val.f = (float)temp_val.i;
    humi_val.f = (float)humi_val.i;
    
    // Calculate Temperature and Humidity
    SHT1x_Calc(&humi_val.f, &temp_val.f);

    *temp = temp_val.f;
    *hum = humi_val.f;

    return 0;
}

int printTempAndHumidity(void)
{
    struct hostent *server;
    struct sockaddr_in serv_addr;
    float temp, hum;

    if (readTempAndHumidity(&temp, &hum) != 0)
    {
        printf("Cannot read temp/humidity\n");
        return 1;
    }

    printf("%0.1f,", temp);
    printf("%0.1f\n", hum);

    exit(0);
}

int sendTempAndHumidity(const char *host, unsigned short port, const char *temp_label, const char *hum_label)
{
    struct hostent *server;
    struct sockaddr_in serv_addr;
    float temp, hum;
    char line1[1024];
    char line2[1024];
    int fd = 0, n = 0;

    if (readTempAndHumidity(&temp, &hum) != 0)
    {
        printf("Cannot read temp/humidity\n");
        return 1;
    }

    //Print the Temperature to the console
    sprintf(line1, "%s %0.2f %ld\n", temp_label, temp, time(NULL));
    printf(line1);

    //Print the Humidity to the console
    sprintf(line2, "%s %0.2f %ld\n", hum_label, hum, time(NULL));
    printf(line2);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("cannot create socket\n");
        return -1;
    }

    server = gethostbyname(host);
    if (server == NULL)
    {
        printf("cannot lookup localhost. thats bad.\n");
        return -1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;    
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
    {
        printf("cannot connect to localhost. Is carbon running?\n");
        return -1;
    }

    if (send(fd,line1,strlen(line1),0) < 0)
    {
        printf("cannot send data to carbon.\n");   
        close(fd);
        return -1;
    }

    if (send(fd,line2,strlen(line2),0) < 0)
    {
        printf("cannot send data to carbon.\n");   
        close(fd);
        return -1;
    }
    close(fd);

    return 0;
}

int main (int argc, char *argv[])
{

    if (argc != 5 && argc != 1)
    {
        printf("Usage: th_logger <server> <port> <temp label> <hum label>\n");
        exit(1);
    }

    if (getuid() != 0)
    {
        printf("This program must be run as root.\n");
        exit(1);
    }

    //Initialise the Raspberry Pi GPIO
    if(!bcm2835_init())
        return 1;

    if (argc == 1)
        printTempAndHumidity();
    else
        return sendTempAndHumidity(argv[1], atoi(argv[2]), argv[3], argv[4]);
}
