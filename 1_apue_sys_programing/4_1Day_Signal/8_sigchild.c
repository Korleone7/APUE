#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sys_err(char *str)
{
    perror(str);
    exit(1);
}

void do_sig_child(int signo)
{
    int status;
    pid_t pid;

//    if ((pid = waitpid(0, &status, WNOHANG)) > 0) {
//    信号不支持排队，当正在执行的SIGCHLD捕捉函数时，再过一个或者多个SIGCHLD信号怎么办？


	//如果是while的话，即使同时死了多个子进程，也会依次把这批死掉的进程进行while处理
	//因为0：不区分的回收所有子进程，WNOHANG不阻塞
    while ((pid = waitpid(0, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status))//进程正常退出？
            printf("------------child %d exit %d\n", pid, WEXITSTATUS(status));//进程状态
        else if (WIFSIGNALED(status))//进程异常终止？
            printf("child %d cancel signal %d\n", pid, WTERMSIG(status));//进程状态
    }
}

int main(void)
{
    pid_t pid;
    int i;
    //阻塞SIGCHLD
    for (i = 0; i < 10; i++) {
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
            sys_err("fork");
    }

    if (pid == 0) {     //10个子进程
        int n = 1;
        while (n--) {
            printf("child ID %d\n", getpid());
            sleep(1);
        }
        return i+1;
    } else if (pid > 0) {//父进程
        //TD：SIGCHLD阻塞，为了防止子进程执行过快，导致还未设置阻捕捉函数
        struct sigaction act;

        act.sa_handler = do_sig_child;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGCHLD, &act, NULL);
        //TD：解除对SIGCHLD的阻塞，用于接受SIGCHLD信号
        
        while (1) {
            printf("Parent ID %d\n", getpid());
            sleep(1);
        }
    }

    return 0;
}

