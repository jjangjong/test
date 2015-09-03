#include <stddef.h>
#include <sys/socket.h>
#include <netdb.h>

#include "netTest.h"

int netDial(struct netTest *test){
	struct addrinfo hints, *local_res, *server_res;
	int s;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = test->domain;
	hints.ai_socktype = test->proto;
	if (getaddrinfo(test->serverHostname, NULL, &hints, &server_res) != 0)
		return -1;

	s = socket(server_res->ai_family, test->proto, 0);
	if (s < 0) {
		freeaddrinfo(local_res);
		freeaddrinfo(server_res);
		return -1;
	}

	((struct sockaddr_in *) server_res->ai_addr)->sin_port = htons(test->serverPort);
	if (connect(s, (struct sockaddr *) server_res->ai_addr,
			server_res->ai_addrlen) < 0) {
		close(s);
		freeaddrinfo(server_res);
		return -1;
	}

	freeaddrinfo(server_res);
	test->ctrlSocket = s;
	return s;
}


int netAnnounce(struct netTest *test){
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
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0) {
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
