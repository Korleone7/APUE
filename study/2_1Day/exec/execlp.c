#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("========================\n");

    char *argvv[] = {"ls", "-l", "-F", "-a", NULL};

    pid_t pid = fork();
	if(pid == -1) {//fork fail
		perror("fork");
		exit(-1);
	}
	else if (pid == 0) {//子进程的fork返回0
		printf("execl...\n");
		//path + produce [for any ur produce]
        execl("/bin/ls", "ls", "-l", "-h", "-a", NULL);
		//$PATH 
		execlp("ls", "ls", "-l", "-h", "-a", NULL);
		//v:cmd arg array
        execv("/bin/ls", argvv);
//        perror("execlp");
        exit(1);

    } 
	else if (pid > 0) {//父进程的fork返回子进程的ID(非负)
        sleep(1);
        printf("I am parent...\n");
    }

	printf("------ finish ---- \n");
    return 0;
}
