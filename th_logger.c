#include <unistd.h>
#include <wiringPi.h>
#include <maxdetect.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>

#define HOST "localhost"
#define PORT 2003

int main ()
{
  int fd;
  int attempts;
  int myTemp = -1;
  int myRelHumidity = -1;
  time_t t;
  struct hostent *server;
  struct sockaddr_in serv_addr;
  char txt1[255], txt2[255];

  if (wiringPiSetupGpio () == -1)
      exit(1);

  for(attempts = 0; attempts < 10; attempts++)
  {
      usleep (1000 * 10);
      if (readRHT03 (18, &myTemp, &myRelHumidity))
         break;
  }

  time(&t); 
  printf("             Time: %lld\n", (long long)t);
  printf("      Temperature: %.1f\nRelative humidity: %.1f\n", (float)myTemp / 10.0, (float)myRelHumidity / 10.0);


  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
  {
      printf("cannot create socket\n");
      return -1;
  }

  server = gethostbyname(HOST);
  if (server == NULL)
  {
      printf("cannot lookup localhost. thats bad.\n");
      return -1;
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;    
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(PORT);

  if (connect(fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
  {
      printf("cannot connect to localhost. Is carbon running?\n");
      return -1;
  }

  sprintf(txt1, "rumpcentral.office.temperature %2.1f %lld\n", (float)myTemp / 10.0, (long long)t);
  if (send(fd,txt1,strlen(txt1),0) < 0)
  {
      printf("cannot send data to carbon.\n");   
      close(fd);
      return -1;
  }

  sprintf(txt2, "rumpcentral.office.humidity %2.1f %lld\n", (float)myRelHumidity / 10.0, (long long)t);
  if (send(fd,txt2,strlen(txt2),0) < 0)
  {
      printf("cannot send data to carbon.\n");   
      close(fd);
      return -1;
  }
  close(fd);
  printf("Data successfully submitted\n");

  return 0;
}
