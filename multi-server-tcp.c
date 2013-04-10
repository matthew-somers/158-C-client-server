/**************************************************************
 * connection-oriented server programming 
 * 
 * Function: iterative echo server 
 * 
 * Reference: Richard Stevens, Unix Network Programming, 1998
 *
 * Created by: Hairong Qi
 **************************************************************/
#include <stdio.h>        /* for standard I/O */
#include <unistd.h>       /* for read() and write() and fork() */
#include <string.h>
#include <strings.h>      /* for bzero() */
#include <stdlib.h>       /* for atoi() */
#include <sys/types.h>    /* for accept() and connect() and fork() */
#include <sys/socket.h>
#include <sys/errno.h>    /* for perror() */
#include <netinet/in.h>   /* for htonl() */

#define LISTENQ 2
#define USAGE "./iservertcp_fork portnumber packetsize\n"

/**
 ** output the error message and exit
 **/
void error(char *msg)
{
	perror(msg);
	exit(1);
}


/**
 ** the main function 
 **/
int main(int argc, char *argv[])
{
	int BUFLEN = atoi(argv[2]);	
	char tempbuf[BUFLEN];
	char sendbuf[BUFLEN];
	char receivebuf[BUFLEN];
	int lefttoreceive, received;

	int listenfd, connfd;              /* socket file descriptor */
	int portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	pid_t childpid;                    /* for fork() */

	/* check the correctness of command-line inputs */
	if (argc < 2) 
		error(USAGE);


	/* create a listening socket */
	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) 
		error("ERROR opening socket");

	/* specify local address */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	portno = atoi(argv[1]);
	serv_addr.sin_port = htons(portno);

	/* bind the address to the listening socket */
	if (bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");

	/* specify a queue length for the server */
	listen(listenfd, LISTENQ);

	while (1) {
		/* accept connections */
		clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr *) &cli_addr, &clilen);
		if (connfd < 0) 
			error("ERROR on accept");

		/* create a child process that handles new client */
		if ((childpid = fork()) == 0) {            /* child process */
			/* first close the listening socket copied from parent */
			close(listenfd);

			/* then child process starts to receive data */
			while (1) {
				bzero(receivebuf, BUFLEN);
				lefttoreceive = BUFLEN;

				while (lefttoreceive > 0)
				{
					received = recv(connfd,tempbuf,BUFLEN,0);
					sprintf(receivebuf + strlen(receivebuf), tempbuf);
					lefttoreceive -= received;
				}

				printf("Message: %s\n" , receivebuf);
				sprintf(sendbuf, "I received: %s", receivebuf);
				send(connfd, sendbuf, BUFLEN, 0);
				/*
				   bzero(buffer, BUFLEN);
				   n = read(connfd, buffer, BUFLEN-1);
				   if (n < 0) 
				   error("ERROR reading from socket");
				   printf("Message: %s\n", buffer);
				   fflush(stdout);

				// start to reply to client 
				n = write(connfd, "I got your message", 18);
				if (n < 0) 
				error("ERROR writing to socket");*/
			}
		}

		/* the parent process closes the connected socket 
		   as the child already has a copy */
		close(connfd);
	}

	return 0; 
}

