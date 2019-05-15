#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printset(sigset_t *ped)
{
	int i;
	for(i = 1; i < 32; i++){
		if((sigismember(ped, i) == 1)){
			putchar('1');
		} else {
			putchar('0');
		}
	}
	printf("\n");
}

int main(void) {
	sigset_t myset, ped;

	sigemptyset(&myset);

#if 0  //add屏蔽字
	sigaddset(&myset, SIGINT);
	sigaddset(&myset, SIGQUIT);
	sigaddset(&myset, SIGKILL);
	sigaddset(&myset, SIGSEGV);
#else
	sigfillset(&myset);//全屏蔽
#endif
	
	//将自定义的屏蔽字映射到进程的信号屏蔽字中
	sigprocmask(SIG_BLOCK, &myset, NULL);	//不获取原屏蔽字

	while(1){
		sigpending(&ped);       //获取未决信号集
		printset(&ped);
		sleep(1);
	}

	return 0;
}



