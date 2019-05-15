#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

void myfunc(int signo)
{
	printf("I am signal func... \n");
   
	//raise(SIGALRM); //这句话会导致主程序频繁捕捉到信号，一直打印
}

int main(void)
{
	struct itimerval it, oldit;
	signal(SIGALRM, myfunc);   //注册SIGALRM信号的捕捉处理函数。

	it.it_value.tv_sec = 5;
	it.it_value.tv_usec = 0;

	it.it_interval.tv_sec = 1;
	it.it_interval.tv_usec = 0;

	if(setitimer(ITIMER_REAL, &it, &oldit) == -1){
		perror("setitimer error");
		return -1;
	}

	while(1);

	return 0;
}
