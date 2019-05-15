#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	int fd;

	//打开已经存在的文件
	fd = open("hello.c", O_RDWR);
	if(fd == -1) {
		perror("open fail\n");
		exit(-1);
	}

	//关闭
	int ret = close(fd);
	printf("ret %d\n", ret);
	if(ret == -1) {
		printf("close fail");
		exit(-1);
	}

	//创建新文件
	fd = open("myhello.txt", O_RDWR | O_CREAT, 0777);
	printf("fd %d\n", fd);
	if(fd == -1) {
		perror("create fail");
		exit(-1);
	}
	
	//判断文件是否存在
	fd = open("myhello.txt", O_RDWR | O_CREAT |  O_EXCL, 0777);
	if(fd == -1) {
		perror("create fail");
		//exit(-1);
	}

	//将文件长度截断为0（清空）
	fd = open("myhello.txt", O_RDWR | O_TRUNC);
	if(fd == -1){
		perror("open fail");
		exit(-1);
	}

	return 0;
}


