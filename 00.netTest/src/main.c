#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/mman.h>

#include "netTest.h"

const char usage_shortstr[] = "Usage: hanTest [-s|-c host] [-p port]\n"
                           "Try `netTest --help' for more information.\n";

void usage() {
	fputs(usage_shortstr, stderr);
}

int parseArguments(struct netTest *test, int argc, char **argv){
	static struct option longopts[] = {
			{"port", required_argument, NULL, 'p'},
			{"client", required_argument, NULL, 'c'},
			{"server", no_argument, NULL, 's'},
			{"bandwidth", required_argument, NULL, 'b'},
			{NULL, 0, NULL, 0}
	};
	int flag;

	while ((flag = getopt_long(argc, argv, "p:sc:b:",longopts, NULL)) != -1){
		switch(flag) {
		case 'p':
			test->bindPort = atoi(optarg);
			break;
		case 's':
			test->role = 's';
			break;
		case 'c':
			test->role='c';
			test->bindAddress = strdup(optarg);
			break;
		case 'b':
			test->blksize = atoi(optarg);
			break;
		default :
			return -1;
		}
	}
	return 0;
}

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
	test->bindPort= 15000;
	test->bindAddress="127.0.0.1";
	test->blksize = DEFAULT_TCP_BLKSIZE;

	return test;
}

int main(int argc, char **argv) {
	struct netTest *test;
	struct stream *sp;

	printf("merge test\n");
	printf("create branch\n");
	printf("gitTest conflict\n");
	printf("gitk test\n");
	printf("hanjong git\n");
	printf("master change\n");
	if(!(test = initTest()))
		printf("Init error\n");

	if(parseArguments(test, argc, argv)<0){
		printf("Error arguments\n");
		usage();
		exit(1);
	}
	printf("test->blksize = %d\n",test->blksize);

	if(!(sp = makeSteam(test))){
		printf("Error makeStream\n");
		exit(1);
	}
	test->sp = sp;

	if(test->role == 's'){
		server(test, sp);
	}else
		client(test, sp);

	//mumap(sp->buffer, test->blksize);
	close(sp->buffer_fd);
	free(test);
	return 0;
}


