#include <arpa/inet.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>

#include "api.h"
#include "debug.h"
#include "net.h"
#include "netTest.h"
#include "timer.h"

#define BUF_SIZE 1024

static jmp_buf sigend_jmp_buf;
static void sigend_handler(int sig) {
	longjmp(sigend_jmp_buf, 1);
}
static void cleanupServer(struct netTest *test){
	/* Close open test sockets */
	close(test->ctrlSocket);
	close(test->listener);
}

int serverRun(struct netTest *test){
	fd_set readSet, writeSet;
	struct timeval *timeout;
	struct timeval now;
	int result;

	/* Termination signals. */
	catchSigend(sigend_handler);
	if (setjmp(sigend_jmp_buf))
		getSigend(test);

	if ((test->listener = netAnnounce(test)) < 0) {
		debug("Can't get socket\n");
		return -1;
	}

	FD_ZERO(&test->readSet);
	FD_ZERO(&test->writeSet);
	FD_SET(test->listener, &test->readSet);
	if(test->listener > test->maxFd)
		test->maxFd = test->listener;

	test->status = NET_PREPARE;

	while (test->status != NET_END){
		memcpy(&readSet, &test->readSet, sizeof(fd_set));
		memcpy(&writeSet, &test->writeSet, sizeof(fd_set));

		(void) gettimeofday(&now, NULL);
		timeout = tmr_timeout(&now);
		result = select(test->maxFd + 1, &readSet, &writeSet, NULL, timeout);

		if(result == 0){
			debug("select timeout\n");
			continue;
		}else if(result <0){
			debug("select error\n");
			cleanupServer(test);
			return -1;
		}

		if(FD_ISSET(test->listener, &readSet)){
			if(netAccept(test) <0){
				cleanupServer(test);
				return -1;
			}
			FD_CLR(test->listener, &readSet);
		}
		if(FD_ISSET(test->ctrlSocket, &readSet)){
			int res = read(test->ctrlSocket, (char*) &test->status, sizeof(signed char));
			printf("test->status = %d\n",test->status);
			if(res<=0){
				if(res == 0) {
					debug("the client has unexpectedly closed the connection\n");
					test->status = NET_END;
					break;
				}else {
					debug("ctrlSocket has problems\n");
					cleanupServer(test);
					return -1;
				}
			}
		}
		switch(test->status){
		case NET_START:



		break;
		case NET_END:

		break;
		default:
		break;
		}
		write(test->ctrlSocket, "write function",14);
	}

	printf("return\n");







//	server(test, test->sp);

	return 0;
}

int server(struct netTest *test, struct stream *sp)
{
	int serv_sock, clnt_sock;
	struct sockaddr_in clnt_adr;
	struct timeval *timeout;
	struct timeval now;
	fd_set reads, cpy_reads;


	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	int handler=-1;


#if 0

	struct sockaddr_in serv_adr;
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(test->bindPort);

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
#else
	serv_sock = test->listener;
#endif

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max=serv_sock;

	while(1)
	{
		cpy_reads=reads;
//		timeout.tv_sec=5;
//		timeout.tv_usec=5000;

		(void) gettimeofday(&now, NULL);
		timeout = tmr_timeout(&now);

		if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, timeout))==-1)
			break;

		if(fd_num<0){
			debug("Server select error\n");
			return -1;
		} else if (fd_num==0){
			debug("Select time out\n");
			continue;
		}



		for(i=0; i<fd_max+1; i++)
		{
			if(FD_ISSET(i, &cpy_reads))
			{
				if(i==serv_sock)     // connection request!
				{
					adr_sz=sizeof(clnt_adr);
					clnt_sock=
						accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if(fd_max<clnt_sock)
						fd_max=clnt_sock;
					printf("connected client: %d \n", clnt_sock);
				}
				else    // read message!
				{
					str_len=read(i, buf, BUF_SIZE);

					printf("buf=%s\n",buf);
					if(!(strncmp("test",buf,4))){
						handler = 0;
						printf("handler test\n");
					}
					else if(!(strncmp("quit",buf,4))){
						handler = 1;
						printf("handler quit\n");
					}else{
						handler =-1;
						printf("handler default\n");
					}

					switch(handler){
					case 0:
						printf("printf test\n");
					break;
					case 1:
						printf("print quit\n");

					break;
					default:
						printf("default\n");
					break;
					}
					handler = -1;

					if(str_len==0)    // close request!
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i);
					}
					else
					{
						write(i, buf, str_len);    // echo!
						printf("client send: %s\n\n",buf);
					}

				}
			}
		}
	}
	munmap(sp->buffer, test->blksize);
	close(serv_sock);
	close(sp->buffer_fd);
	return 0;
}
