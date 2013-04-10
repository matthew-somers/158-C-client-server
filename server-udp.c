#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9930

//1 byte, 1KB, 4KB, 8 KB, 16KB, 32 KB, and 64KB
//#define BUFLEN 1
//#define BUFLEN 1024
#define BUFLEN 4096
//#define BUFLEN 8192
//#define BUFLEN 16384
//#define BUFLEN 32768
//#define BUFLEN 64000 //because full 64KB might not be supported

int main(void)
{
   struct sockaddr_in si_me, si_other;
   int s, i, slen=sizeof(si_other);
   char buf[BUFLEN];

   // socket()
   s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

   //complicated stuff for receiving
   memset((char *) &si_other, 0, sizeof(si_other));
   si_other.sin_family = AF_INET;
   si_other.sin_port = htons(PORT);

   //complicated stuff for sending acks back
   memset( (char *) &si_me, 0, sizeof(si_me) );
   si_me.sin_family = AF_INET;
   si_me.sin_port = htons(PORT);
   si_me.sin_addr.s_addr = htonl(INADDR_ANY);

   //bind()
   bind(s, &si_me, sizeof(si_me));


   //main loop, waits, and the sends ack when it receives something
   while (1) 
   {
      recvfrom(s, buf, BUFLEN, 0, &si_other, &slen);

      printf("Received packet from %s:%d\nData: %s", 
         inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
      
      printf("Sending ack to %s\n\n", inet_ntoa(si_other.sin_addr));

      sendto(s, buf, BUFLEN, 0, &si_other, slen);
   }

   close(s);
   return 0;
}
