#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	pid_t pid;
	int fd[2], i, n;
	char buf[1024];

	int ret = pipe(fd);
	if(ret == -1){
		perror("pipe error");
		exit(1);
	}

	for(i = 0; i < 3; i++){
		if((pid = fork()) == 0) {//子进程出口
			break;
		}
		else if(pid == -1){
			perror("pipe error");
			exit(1);
		}
	}

	if (i == 0) {//子进程1 写端			
		printf("I am child1 W\n");
		close(fd[0]);				
		write(fd[1], "Child1 W2 Vs R1\n", strlen("Child1 W1 Vs R2\n"));
	} else if(i == 1) {//子进程2 写端		
		printf("I am child2 W\n");
		close(fd[0]);				
		write(fd[1], "Child2...\n", strlen("Child2...\n"));
	} else if(i == 2){//子进程3 读端
		printf("I am child3 R\n");
		close(fd[1]);
		n = read(fd[0], buf, 1024);	
		write(STDOUT_FILENO, buf, n);
	} 
	else {//父进程 读端
		printf("I am parent R\n");
		close(fd[1]);//父进程关闭写端,留读端读取数据    
		sleep(1);
		n = read(fd[0], buf, 1024);     //从管道中读数据
		write(STDOUT_FILENO, buf, n);

		for(i = 0; i < 2; i++)		//两个儿子wait两次
			wait(NULL);
	}

	return 0;
}
