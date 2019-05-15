#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* 值得说明的是
 *
 * Ctrl+C(SIGINT) 是终止进程
 * Ctrl+\(SIGQUIT) 是终止终端所启动的程序
 * Ctrl+Z(SIGTSTP) 停止终端交互进程的运行
 * */

/*自定义的信号捕捉函数*/
void sig_int(int signo)
{
	printf("catched No.%d signal...\n", signo);//单次打印
    sleep(7);//这段时间内输入SIQUIT SIGINT SIGTSTP都无效，但是在程序结束后会生效
    printf("----slept 7s\n");
}

int main(void)
{
	struct sigaction act;		

	//act初始化
	//1.注册函数
	act.sa_handler = sig_int;
	//2.默认值
	act.sa_flags = 0;
	//3.mask信号屏蔽字
	sigemptyset(&act.sa_mask);//不屏蔽任何信号
	sigaddset(&act.sa_mask, SIGQUIT);//增加SIGQUIT到信号集

	//信号捕捉函数
	sigaction(SIGINT, &act, NULL);//捕捉SIGINT后，处理act.sa_handler期间，屏蔽act.sa_mask的干扰

    //printf("------------main slept 11s\n");
    //sleep(11);

	while(1);//该循环只是为了保证有足够的时间来测试函数特性

	return 0;
}


/*==========================================================================*/

