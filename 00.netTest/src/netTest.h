
#ifndef NETTEST_H_
#define NETTEST_H_

#define DEFAULT_TCP_BLKSIZE (128 * 1024)  /* default read/write block size */

enum {
	NET_PREPARE = 0,
	NET_START,
	NET_TESTING,
	NET_END,
	NET_SERVER_TERMINATE,
	NET_CLIENT_TERMINATE,
	ACCESS_DENIED =(-1),
};


struct netTest {
	char role;				// 'c': client, 's': server
	char *bindAddress;
	char *serverHostname;// server hostname or IP
	int serverPort;		// server port
	signed char status;			// program status
	int domain;			// AF_INET or AF_INET6

	int listener;
	int ctrlSocket;

	int blksize;
	int proto;				// SOCK_STREAM: TCP, SOCK_DGRAM: UDP

	/*Select related parameters */
	int maxFd;
	fd_set readSet;		// set of read sockets
	fd_set writeSet;		// set of write sockets

	char debug;

	struct stream *sp;

};

struct stream {
	struct netTest* test;

	char *buffer;
	int buffer_fd;
};


extern char debugging;
extern void debug(const char * format, ...);

#endif /* NETTEST_H_ */
