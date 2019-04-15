#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    int i;
    pid_t pid;
    printf("-------------------------\n");

    for (i = 0; i < 5; i++) {
		printf("Creating No.%d process\n", i);	
        pid = fork();
        if (pid == 0) {//父进程返回子进程的pid，子进程返回0
            break;
        }
    }

    if (i >= 5) {
        sleep(i);
        printf("I'm parent, pid = %u, my parent ID: %u\n", getpid(), getppid());
    } else if(i < 5) {
		sleep(i);
        printf("I'am %d child , pid = %u, my parent ID: %u\n", i+1, getpid(), getppid());
    }
	
	printf("========================\n");

    return 0;
}

/***********************************
 * 由父进程创建完子进程后，
 * 父进程和子进程具有相同的地位去争取CPU
 *
 * 在shell终端运行a.out程序时，shell会切到终端后台，把终端前台交给a.out
 * 当a.out执行完毕后(return 0)，shell再从后台获得前台权限
 *
 * 因此，如果不加sleep(i)，如果父进程提前子进程结束，就会导致shell进程，父进程，子进程混在一起
 *
 * ×××××××××××××××××××××××××××××××××××××××××××××××××××××
 * 进程共享：读时共享，写时复制
 *
 * ×××××××××××××××××××××××××××××××××××××××××××××××××××××
 * gdb调试
 * set follow-fork-mode parent [default]
 * set follow-fork-mode child 
 *
 * ×××××××××××××××××××××××××××××××××××××××××××××××××××××
 *
 *
-------------------------
Creating No.0 process
Creating No.1 process
Creating No.2 process
I'am 1 child , pid = 21418
========================
Creating No.3 process
Creating No.4 process
I'am 2 child , pid = 21421
========================
I'am 3 child , pid = 21422
========================
I'am 4 child , pid = 21423
========================
I'am 5 child , pid = 21424
========================
I'm parent
========================
************************************/











