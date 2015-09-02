
#ifndef NETTEST_H_
#define NETTEST_H_

#define DEFAULT_TCP_BLKSIZE (128 * 1024)  /* default read/write block size */

struct netTest {
	char role;				// 'c': client, 's': server
	char *bindAddress;	// bind Address
	int bindPort;			// bind port


	int blksize;

	struct stream *sp;
};

struct stream {
	char *buffer;
	int buffer_fd;
};

#endif /* NETTEST_H_ */
