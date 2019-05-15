#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int n = 5, i;				//默认创建5个子进程
    pid_t p, q, wpid;

	if(argc == 2){	
		n = atoi(argv[1]);
	}

	for(i = 0; i < n; i++)	 {//出口1,父进程专用出口
        p = fork();
		if(p == 0) {
			break;			//出口2,子进程出口,i不自增
        } else if (i == 3){
            q = p;
        }
    }

	if(n == i){
		sleep(n);
		printf("I am parent, pid = %d\n", getpid());

		//DEMO 回收全部子进程，利用参数3的0阻塞并等待子进程结束
		//等价于wait
		//参数1 
		//	>0: 回收指定的子进程ID
		//	-1: 回收任一子进程
		//while(waitpid(-1, NULL, 0));// while(wait(NULL));

//--------------------------------------------------------------------------

		//DEMO 利用参数3的WNOHANG轮循子进程是否结束
		do{
			//参数3
			//	指定为WNOHANG，如果子进程没有结束，waitpid返回0
			//	指定为0:表示阻塞，如果子进程没有结束，waitpid不返回
			wpid = waitpid(-1, NULL, WNOHANG);
			//wpid返回值
			//	>0 : 成功清理掉的子进程PID
			//	-1 : 清理失败，无子进程
			//	0  : 指定参数为WNOHANG时，子进程没结束，返回0
			if(wpid > 0) {
				n--;
				printf("n = %d\n", n);
			}
			sleep(1);
		}while(n  > 0);

		printf("waitpid finish\n");
               //pid_t pid = wait(NULL);
        //printf("child pid = %d\n", pid);
        while(1);

	} else {
		sleep(i);
		printf("I'm %dth child, pid = %d, gpid=%d\n", 
				i+1, getpid(), getgid());
	}

	return 0;
}
