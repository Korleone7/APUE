#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void donothing(int signo)
{
}

unsigned int mysleep(unsigned int seconds) 
{
    unsigned int ret;

    struct sigaction act, oldact;
    act.sa_handler = donothing;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, &oldact);

    //SIGALRM 阻塞
    alarm(seconds); //定时固定的秒数  1 
//------失去了CPU-----  2s后我再次得到CPU
    pause(); //挂起
//SIGALRM 解除阻塞


    ret = alarm(0);  
    sigaction(SIGALRM, &oldact, NULL);  //恢复SIGALRM 默认处理方式

    return ret;
}

int main(void)
{
    mysleep(1);
    printf("slept %d s\n", 5);

    return 0;
}

