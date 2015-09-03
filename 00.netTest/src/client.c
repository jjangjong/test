#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "debug.h"
#include "net.h"
#include "netTest.h"
#include "timer.h"
#define SEC_TO_US 1000000LL
#define SEC_TO_MS 1000L

#define BUF_SIZE 1024

#ifndef SOCKET
int clientsocket;
char message[DEFAULT_TCP_BLKSIZE];
int str_len;
int interval=1000;
char *data;
#endif

void timer_proc1(TimerClientData client_data, struct timeval *nowP){
//	write(clientsocket, data, client_data.p);

	fputs("Input message(Q to quit): ", stdout);
	fgets(message, BUF_SIZE, stdin);

	write(clientsocket, message, strlen(message));
	str_len = read(clientsocket, message, BUF_SIZE - 1);
	message[str_len] = 0;
	printf("Message from server: %s", message);
}

int client(struct netTest *test, struct stream *sp)
{
#ifdef SOCKET
	int clientsocket;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;

	clientsocket=socket(PF_INET, SOCK_STREAM, 0);
	if(clientsocket==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(test->bindAddress);
	serv_adr.sin_port=htons(test->bindPort);

	if(connect(clientsocket, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");


	while(1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

		write(clientsocket, message, strlen(message));
		str_len=read(clientsocket, message, BUF_SIZE-1);
		message[str_len]=0;
		printf("Message from server: %s", message);

	}

	close(clientsocket);
#else

	//for timer
	struct timeval now;
//	struct timeval* timeout;
	Timer *test1;
	TimerClientData cd;


	struct sockaddr_in serv_adr;

	/////////////////////////////
	// tmp
	char msg[10]="test\n";

	data = (char *)malloc(sizeof(char*)*test->blksize);
	data = test->sp->buffer;
	cd.p = test->blksize;

	////////////////////////////

#if 0
	clientsocket = socket(AF_INET, SOCK_STREAM, 0);	// PF_INET
	printf("clientsocket =%d\n",clientsocket);
	if (clientsocket == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(test->serverHostname);
	serv_adr.sin_port = htons(test->serverPort);


	if (connect(clientsocket, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	printf("after connect\n");
#else
	if((clientsocket = netDial(test))<0){
		debug("Client connection error\n");
		return -1;
	}
#endif

	(void) gettimeofday(&now, NULL);

	test1 = tmr_create(&now,timer_proc1,cd, interval*SEC_TO_MS,1);
	if(test1 == NULL){
		debug("timer can't create\n");
		return -1;
	}

	while(1){
		(void) gettimeofday(&now, NULL);
		tmr_run(&now);
	}
#endif

	munmap(sp->buffer, test->blksize);
	close(sp->buffer_fd);
	return 0;
}
