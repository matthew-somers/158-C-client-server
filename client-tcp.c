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
   char sendbuf[BUFLEN];
   char receivebuf[BUFLEN];
   char tempbuf[BUFLEN];
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
   int lefttoreceive;
   int received;

   //timing
   double starttime = get_time_ms();


   //sending loop
   for (i = 0; i < NUMPACKETS; i++)
   {
      bzero(receivebuf, BUFLEN);
      sprintf(sendbuf, "This is packet %d\n", i);

      //send through stream
      send(sockfd, sendbuf, BUFLEN, 0);

      lefttoreceive = BUFLEN;
      while (lefttoreceive > 0)
      {
         received = recv(sockfd, tempbuf, BUFLEN,0);
         lefttoreceive -= received;
         sprintf(receivebuf + strlen(receivebuf), tempbuf);
      }

      printf("%s\n", receivebuf);
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
