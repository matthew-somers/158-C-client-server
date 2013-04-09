//changes for each person
#define SRV_IP ""
//#define SRV_IP "98.248.54.14"
   
#include <stdlib.h> /* exit */
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
     
#define NPACK 100 //number of packets to send
#define TIMEOUT 4; //seconds to wait for ack before moving on
#define PORT 9930

//1 byte, 1KB, 4KB, 8 KB, 16KB, 32 KB, and 64KB
//#define BUFLEN 1
#define BUFLEN 1024
//#define BUFLEN 4096
//#define BUFLEN 8192
//#define BUFLEN 16384
//#define BUFLEN 32768
//#define BUFLEN 65536
    
double get_time_ms();
 
int main(void)
{
   struct sockaddr_in si_other, si_me;
   int s, i, slen=sizeof(si_other);
   char buf[BUFLEN];
   char buf2[BUFLEN]; //second buffer to receive acks
     
   //socket()
   s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
     
   //complicated stuff for sending
   memset((char *) &si_other, 0, sizeof(si_other));
   si_other.sin_family = AF_INET;
   si_other.sin_port = htons(PORT);
     
   //complicated stuff for binding and receiving
   memset( (char *) &si_me, 0, sizeof(si_me) );
   si_me.sin_family = AF_INET;
   si_me.sin_port = htons(PORT);
   si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
   //bind() because it needs to receive
   bind(s, &si_me, sizeof(si_me));
     
   //inet_aton() converts ip at top of page
   inet_aton(SRV_IP, &si_other.sin_addr);
     
   //timeout
   struct timeval tv;
   tv.tv_sec = TIMEOUT; 
   setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

   int lostpackets = 0;
   
   //start clock
   double starttime = get_time_ms();

   //main loop, sends a packet, waits for an ack
   for (i=0; i<NPACK; i++)
   {
      printf("Sending packet %d\n", i);
      sprintf(buf, "This is packet %d\n", i);

      sendto(s, buf, BUFLEN, 0, &si_other, slen);
      recvfrom(s, buf2, BUFLEN, 0, &si_other, &slen);

      if (strcmp(buf, buf2) == 0) //buffers match, no packet lost
      {
         printf("Received ack from %s:%d\nData: %s\n", 
           inet_ntoa(si_other.sin_addr),
           ntohs(si_other.sin_port), buf2);
      }
      else
      {
         lostpackets++;
         printf("Packet lost.\n\n");
      }
   }
    
   //end clock
   double endtime = get_time_ms();
   double resulttime = (endtime - starttime) / NPACK;

   printf("Average RTT: %f milliseconds\n", resulttime);
   printf("Packets lost: %d\n", lostpackets);

   close(s);
}

double get_time_ms()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}


