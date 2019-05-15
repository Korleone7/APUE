#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int fd  = open("lseek_txt.txt", O_RDWR);
	if(fd == -1) {
		perror("open fail");
		exit(-1);
	}

	int ret = lseek(fd, 0, SEEK_END);
	printf("file len %d\n", ret);

	//file extension
	ret = lseek(fd, 2000, SEEK_END);
	//achieve it, need take a write operation
	write(fd, "FILE END", 10);

	close(fd);

	return 0;
}

