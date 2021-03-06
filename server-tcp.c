/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
   perror(msg);
   exit(1);
}

int main(int argc, char *argv[])
{
   int sockfd, newsockfd, portno;
   socklen_t clilen;
   struct sockaddr_in serv_addr, cli_addr;
   int lefttoreceive;
   if (argc != 4) 
   {
      fprintf(stderr,"usage: %s port numpackets packetsize \n",
         argv[0]);
      exit(0);
   }
   
   int BUFLEN = atoi(argv[3]);
   char tempbuf[BUFLEN];
   char sendbuf[BUFLEN];
   char receivebuf[BUFLEN];

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
   {
      //close(sockfd);
      error("ERROR opening socket. Should be fixed now?");
   }
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   //bind()
   if (bind(sockfd, (struct sockaddr *) &serv_addr,
      sizeof(serv_addr)) < 0) 
   {
      close(sockfd);
      //error("ERROR on binding");
   }


   //while(1)
   //{
      listen(sockfd,2); //max queue 2
      clilen = sizeof(cli_addr);
      newsockfd = accept(sockfd, 
         (struct sockaddr *) &cli_addr, &clilen);
      if (newsockfd < 0)
      {
         close(newsockfd);
         error("ERROR on accept");
      }

      int i;
      int received;
      int NUMPACKETS = atoi(argv[2]);

      for (i = 0; i < NUMPACKETS; i++)
      {
         bzero(receivebuf, BUFLEN);
         lefttoreceive = BUFLEN;

         while (lefttoreceive > 0)
         {
            received = recv(newsockfd,tempbuf,BUFLEN,0);
            sprintf(receivebuf + strlen(receivebuf), tempbuf);
            lefttoreceive -= received;
         }
         
         printf("Message %d: %s\n", i, receivebuf);
         sprintf(sendbuf, "I received: %s", receivebuf);
         send(newsockfd, sendbuf, BUFLEN, 0);
      }

   close(newsockfd);
   close(sockfd);
   return 0; 
}
