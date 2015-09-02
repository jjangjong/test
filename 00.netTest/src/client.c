#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "netTest.h"
#include "timer.h"
#define SEC_TO_US 1000000LL
#define SEC_TO_MS 1000L

#define BUF_SIZE 1024
//void error_handling(char *message);


#ifndef SOCKET
int sock;
char message[DEFAULT_TCP_BLKSIZE];
int str_len;
int interval=1000;
char *data;

#endif

void timer_proc1(TimerClientData client_data, struct timeval *nowP){
//	write(sock, client_data.p, strlen(client_data.p));
	write(sock, data, client_data.p);
	str_len = read(sock, message, BUF_SIZE - 1);
	message[str_len] = 0;
	printf("str_len = %d\n",str_len);
	printf("Message from server: %s\n", message);
}

int client(struct netTest *test, struct stream *sp)
{
#ifdef SOCKET
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;

	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(test->bindAddress);
	serv_adr.sin_port=htons(test->bindPort);

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");


	while(1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

		write(sock, message, strlen(message));
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
		printf("Message from server: %s", message);

	}

	close(sock);
#else

	//for timer
	struct timeval now;
//	struct timeval* timeout;
	Timer *test1;
	TimerClientData cd;



//	int sock;
//	char message[BUF_SIZE];
//	int str_len;
	struct sockaddr_in serv_adr;

	/////////////////////////////
	// tmp
	char msg[10]="test";
	//cd.p = msg;

	data = (char *)malloc(sizeof(char*)*test->blksize);
	//data= sp->buffer;
	data= test->sp->buffer;
	printf("strlen(data) = %d\n",strlen(data));
	printf("strlen(sp->buffer) = %d\n",strlen(sp->buffer));

	cd.p = test->blksize;
	////////////////////////////


	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(test->bindAddress);
	serv_adr.sin_port = htons(test->bindPort);

	if (connect(sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected...........");


	(void) gettimeofday(&now, NULL);
//	timeout = tmr_timeout(&now);

	//test1 = tmr_create(&now,timer_proc1,cd, interval*SEC_TO_US,1);
	test1 = tmr_create(&now,timer_proc1,cd, interval*SEC_TO_MS,1);
	if(test1 ==NULL)
		printf("****tmr1 error\n");

	while(1){
		(void) gettimeofday(&now, NULL);
		tmr_run(&now);
	}
#endif



	return 0;
}
