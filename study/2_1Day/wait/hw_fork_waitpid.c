/************************************************************************
 *父进程fork三个子进程
 	1.调用ps命令
	2.调用自定义子程序1(正常)
	3.调用自定义子程序2(异常段错误)
  
  最后回收三个子进程
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int n = 3, i = 0;				//默认创建3个子进程
	int fd;	
    pid_t p, wpid;
	int status;

	if(argc == 2){	
		n = atoi(argv[1]);
	}

	for(i = 0; i < n; i++) {//出口1,父进程专用出口
		sleep(i);
        p = fork();
		if(p == -1) {
			perror("fork err");
			exit(-1);		
        }
		if(p > 0) {
			//printf("\nI am parent i = %d\n", i);
		}
		if(p == 0) {
			//bug: 因为arr[]默认是0，如果直接使用i而非i+1，判断的时候，初始值一直卡在arr[0] = 0这个位置
			//printf("I am No.%d child \n", i + 1);
			break;
		}
	}
	//这里其实使用 i = 0 1 2 判断就可以
	//printf("i = %d\n", i);
	if(n == i){
		sleep(n);
		printf("I am parent, pid = %d\n", getpid());

		//DEMO 利用参数3的WNOHANG轮循子进程是否结束
		do{
			//参数3
			//	指定为WNOHANG，如果子进程没有结束，waitpid返回0
			//	指定为0:表示阻塞，如果子进程没有结束，waitpid不返回
			wpid = waitpid(-1, &status, WNOHANG);
			//wpid返回值
			//	>0 : 成功清理掉的子进程PID
			//	-1 : 清理失败，无子进程
			//	0  : 指定参数为WNOHANG时，子进程没结束，返回0
			if(wpid > 0) {
				n--;
				printf("n = %d\n", n);
			}
			
			if(WIFEXITED(status)) {
				printf("child exit with %d\n", WEXITSTATUS(status));
			}
			else if(WIFSIGNALED(status)) {
				printf("child killed with %d \n", WTERMSIG(status));
			}
			sleep(1);
		}while(n  > 0);//do while

		printf("waitpid finish\n");
        while(1);
	}//if(n == i)
	else if(i == 0) {//1号子进程 调用ps
		printf("I am 0nd child, ID :%d \n", getpid());
		fd = open("ps.out", O_WRONLY|O_CREAT|O_TRUNC, 0644);
		if(fd < 0){
			perror("open ps.out error");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);

		execlp("ps", "ps","-a", "-u", "-x", NULL);
		perror("execlp ps err");
	} else if(i == 1) {//2号子进程 调用正常子程序
		printf("I am 1nd child, ID :%d \n", getpid());
		execl("./hw_t", "hw_t", NULL);
		perror("execlp hw_t err");
	} else if(i == 2) {//3号子进程 调用段错误子程序
		printf("I am 2nd child, ID :%d \n", getpid());
		execl("./abnor", "abnor", NULL);
		perror("execlp abnor err");
	}

	return 0;
}

