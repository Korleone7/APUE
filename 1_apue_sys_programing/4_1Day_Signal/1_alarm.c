#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int i;
	//一个进程有且只有一个alarm
	//alarm采用的是自然定时法，与进程状态无关。
	//计时结束后，进程收到SIGALRM信号后，默认动作终止
	alarm(1);

	for(i = 0; ; i++)
		printf("%d\n", i);

	return 0;
}
