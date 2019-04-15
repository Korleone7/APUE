#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	int fd;

	//打开已经存在的文件
	fd = open("myhello.txt", O_RDWR);
	if(fd == -1) {
		perror("open fail\n");
		exit(-1);
	}


	//创建新文件 - 写操作
	int fd_new = open("newfile.txt", O_RDWR | O_CREAT, 0664);
	if(fd == -1) {
		perror("create fail");
		exit(-1);
	}
	
	//read file
	char buf[2048] = {0};
	int count = read(fd, buf, sizeof(buf));
	if(count == -1) {
		perror("read fail\n");
	}

	//write file
	while(count) {
		int ret = write(fd_new, buf, count);
		printf("write byte %d \n", ret);
		//continue read file
		count = read(fd, buf, sizeof(buf));
	}

	//close 
	close(fd);
	close(fd_new);
	return 0;
}





