#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void die(char *info){
	perror(info);
	exit(-1);
}

int
main(void)
{
	char buf[27]  = {'\0'};
	char buf2[27] = {'\0'};
	int  fd, ret;
	
	/*read random from /dev/urandom*/
	fd = open("/dev/urandom", O_RDONLY);	
	if (fd < 0) {
		die("open");
	}
	if ( (ret = read(fd, buf, 26) ) == 0) {
		die("read");
	}
	close(fd);
		
	fd = open("/dev/firebroo", O_RDWR);
	if (fd < 0) {
		die("open");
	}
	printf("open /dev/firebroo successful\n");

	if ( (ret = read(fd, buf2, 26)) == 0) {
		die("read");
	}
	printf("read from kernel successful: [%s]\n", buf2);

	if ( (ret = write(fd, buf, 26)) < 0) {
		die("write");
	}
	if (ret = 26) {
		printf("write to kernel successful: [%s]\n", buf); 
	}

	close(fd);
	return 0;
}
