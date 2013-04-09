#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/time.h>
#include <time.h>

double get_time_ms();

void error(const char *msg)
{
   perror(msg);
   exit(0);
}

int main(int argc, char *argv[])
{
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;


   if (argc != 5) {
      fprintf(stderr,"usage: %s ip port numpackets packetsize \n", argv[0]);
      exit(0);
   }

   int BUFLEN = atoi(argv[4]);
   char buffer[BUFLEN];
   portno = atoi(argv[2]);

   //open socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) 
       error("ERROR opening socket");
   server = gethostbyname(argv[1]);
   if (server == NULL) 
   {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, 
      (char *)&serv_addr.sin_addr.s_addr,
      server->h_length);
   serv_addr.sin_port = htons(portno);
   
   //connect()
   if (connect(sockfd,(struct sockaddr *) &serv_addr,
      sizeof(serv_addr)) < 0)
   {
      error("ERROR connecting");
   }

   int NUMPACKETS = atoi(argv[3]);
   int i;

   //timing
   double starttime = get_time_ms();

   //sending loop

   for (i = 0; i < NUMPACKETS; i++)
   {
      sprintf(buffer, "This is packet %d\n", i);

      //send through stream
      n = write(sockfd, buffer, BUFLEN);
      if (n < 0) 
         error("ERROR writing to socket");
      bzero(buffer, BUFLEN);

      //read ack
      n = read(sockfd, buffer, BUFLEN);
      if (n < 0) 
         error("ERROR reading from socket");
      else
         printf("%s\n", buffer);
   }

   //end clock
   double endtime = get_time_ms();
   double resulttime = (endtime - starttime) / NUMPACKETS;

   printf("Average RTT: %f milliseconds\n", resulttime);
   close(sockfd);
   return 0;
}

double get_time_ms()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}
