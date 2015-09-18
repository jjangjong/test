#include <stddef.h>
#include <sys/socket.h>
#include "netTest.h"

struct netTest *initTest(){
	struct netTest *test;

	test = (struct netTest *)malloc(sizeof(struct netTest));
	if(!test){
		printf("Memory not allocation\n");
		return NULL;
	}
	memset(test, 0, sizeof(struct netTest));

	// default settings
	test->role = 's';
	test->serverPort= 15000;
	test->serverHostname="127.0.0.1";
	test->blksize = DEFAULT_TCP_BLKSIZE;
	test->status = NET_END;
	test->debug = 0;
	test->domain = AF_INET;
	test->proto = SOCK_STREAM;
	test->ctrlSocket = -1;

	return test;
}
