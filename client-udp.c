#define SRV_IP "www.fark.com"
     
#include <stdlib.h> /* exit */
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
     
#define BUFLEN 512
#define NPACK 20
#define PORT 9930
     
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
     
   struct timeval tv1, tv2, result;
   gettimeofday(&tv1,NULL);

    //      time_t t1,t2;
    //      (void) time(&t1);
   //main loop, sends a packet, waits for an ack
   for (i=0; i<NPACK; i++) {
      printf("Sending packet %d\n", i);
      sprintf(buf, "This is packet %d\n", i);
      sendto(s, buf, BUFLEN, 0, &si_other, slen);
     
      recvfrom(s, buf2, BUFLEN, 0, &si_other, &slen);

      printf("Received ack from %s:%d\nData: %s\n\n", 
        inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf2);
      //printf("RTT = %d", t2-t1);
   }
     
    //      (void) time(&t2);
    //      printf("%f", (float)((t2-t1)/));
   gettimeofday(&tv2,NULL);
   timersub(&tv2, &tv1, &result);
   result.tv_sec = result.tv_sec / NPACK;
   result.tv_usec = result.tv_usec / NPACK;
   int milliseconds = (int)result.tv_usec / 1000;
   printf("Average RTT: %u microseconds, %u milliseconds\n",
      (int)result.tv_usec, milliseconds);
   close(s);
   return 0;
}

