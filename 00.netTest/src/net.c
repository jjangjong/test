#include <stddef.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netdb.h>

#include "netTest.h"

/* netDial and netAnnouce code comes from libtask: http://swtch.com/libtask/
 * Copyright: http://swtch.com/libtask/COPYRIGHT
 */
int netDial(struct netTest *test) {
	struct addrinfo hints, *local_res, *server_res;
	int s;
	/* [hjjang] Change source */
	int domain, proto, port;
	char *server;

	domain = test->domain;
	proto = test->proto;
	port = test->serverPort;
	server = test->serverHostname;
	/******************************/

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = domain;
	hints.ai_socktype = proto;
	if (getaddrinfo(server, NULL, &hints, &server_res) != 0)
		return -1;

	s = socket(server_res->ai_family, proto, 0);
	if (s < 0) {
		freeaddrinfo(local_res);
		freeaddrinfo(server_res);
		return -1;
	}

	((struct sockaddr_in *) server_res->ai_addr)->sin_port = htons(port);
	if (connect(s, (struct sockaddr *) server_res->ai_addr,
			server_res->ai_addrlen) < 0) {
		close(s);
		freeaddrinfo(server_res);
		return -1;
	}

	freeaddrinfo(server_res);
	/* [hjjang] Change source */
	//test->ctrlSocket = s;
	return s;
}

int netAnnounce(struct netTest *test) {
	struct addrinfo hints, *res;
	char portstr[6];
	int s, opt, proto = test->proto;	//SOCK_STREAM is TCP

	snprintf(portstr, 6, "%d", test->serverPort);
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = test->domain;
	hints.ai_socktype = proto;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(test->bindAddress, portstr, &hints, &res) != 0)
		return -1;

	s = socket(res->ai_family, proto, 0);
	if (s < 0) {
		freeaddrinfo(res);
		return -1;
	}

	opt = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt))
			< 0) {
		close(s);
		freeaddrinfo(res);
		return -1;
	}

	if (bind(s, (struct sockaddr *) res->ai_addr, res->ai_addrlen) < 0) {
		close(s);
		freeaddrinfo(res);
		return -1;
	}

	freeaddrinfo(res);

	if (proto == SOCK_STREAM) {
		if (listen(s, 5) < 0) {
			close(s);
			return -1;
		}
	}
	return s;

}

int netAccept(struct netTest *test) {
	int s;
	signed char rbuf = ACCESS_DENIED;
	socklen_t len;
	struct sockaddr_storage addr;

	len = sizeof (addr);
	if ((s = accept(test->listener, (struct sockaddr *) &addr, &len)) < 0) {
		debug("accept error\n");
		return -1;
	}
	printf("s = %d\n",s);

	if(test->ctrlSocket == -1){
		test->ctrlSocket = s;
		FD_SET(test->ctrlSocket, &test->readSet);
		if(test->ctrlSocket > test->maxFd)
			test->maxFd = test->ctrlSocket;
	}else{
		if(write(s, (char*) &rbuf, sizeof(rbuf))<0){
			debug("ACCESS_DINIED send error\n");
			return -1;
		}
		debug("ACCESS_DINEID\n");
		close(s);
	}

	return 0;
}
