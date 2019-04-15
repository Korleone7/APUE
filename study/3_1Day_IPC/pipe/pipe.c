#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(void)
{
    pid_t pid;
    char buf[1024];
    int fd[2];
    char *p = "test for pipe\n";
    
   if (pipe(fd) == -1) 
       sys_err("pipe");

   pid = fork();
   if (pid < 0) {
       sys_err("fork err");
   } else if (pid == 0) {//子进程 关写fd[1] 读数据
        close(fd[1]);
        int len = read(fd[0], buf, sizeof(buf));
		printf("I am child...\n");
        write(STDOUT_FILENO, buf, len);
        close(fd[0]);
   } else {// 父进程 关读fd[0] 写数据
       close(fd[0]);
	   sleep(3);
       write(fd[1], p, strlen(p));
       printf("I am parent\n");
       close(fd[1]);
   }
    
    return 0;
}
