
#ifndef NETTEST_H_
#define NETTEST_H_

#define DEFAULT_TCP_BLKSIZE (128 * 1024)  /* default read/write block size */

enum {
	NET_START = 0,
	NET_END,
	NET_DOING,
};


struct netTest {
	char role;				// 'c': client, 's': server
	char *bindAddress;
	char *serverHostname;// server hostname or IP
	int serverPort;		// server port
	char status;			// program status

	int domain;			// AF_INET or AF_INET6

	int ctrlSocket;
	int listener;



	int blksize;
	int proto;				// SOCK_STREAM: TCP, SOCK_DGRAM: UDP


	int maxFd;
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
