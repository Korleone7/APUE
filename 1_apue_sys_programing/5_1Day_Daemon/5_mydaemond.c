#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* ***********************
 * 守护进程初始化操作
 ************************/
void daemonize(void)
{
    pid_t pid;
	//1.创建子进程 fork
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid != 0) /* parent */
        exit(0);

	//2.子进程创建新会话，成为会长兼任进程组组长，并失去控制终端
    setsid();

    //3.改变当前工作目录到/目录下
    if (chdir("/") < 0) {
        perror("chdir");
        exit(1);
    }

    //4.设置umask为0
    umask(0002);

    //5.重定向0，1，2文件描述符到 /dev/null，因为已经失去控制终端，再操作0，1，2没有意义.
    close(0);//close(STDIN_FILENO);
    open("/dev/null", O_RDWR);//0指向null文件
    dup2(0, 1);//dup2(0, STDOUT_FILENO);
    dup2(0, 2);//dup2(0, STDERR_FILENO);
}

/************************************************
 * 本程序运行完，可以用ps ajx/aux找到
 * 守护进程在后台运行，不受用户登录、注销的影响
 ***********************************************/
int main(void) {
    daemonize();
    while(1); /* 在此循环中可以实现守护进程的核心工作 */
}




