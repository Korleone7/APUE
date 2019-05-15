#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void *tfn(void *arg)
{
	printf("I'm thread, Thread_ID = %lu, pid = %u\n", pthread_self(), getpid());
	return NULL;
}

int main(void)
{
	pthread_t tid;
	int ret;
	
	//主控线程ID
	printf("I am main1: my thread_ID : %lu and my pid = %d\n", pthread_self(), getpid());

	//线程创建完毕后，就会直接调用tfn去执行
	ret = pthread_create(&tid, NULL, tfn, NULL);
	if(ret != 0){
		fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
		exit(-1);
	}
	//sleep(1);
	
	printf("I am main2: my thread_ID : %lu and my pid = %d\n", pthread_self(), getpid());

	//return 0;
	
	//将当前主控线程退出，而创建的线程不会受影响，因为只要有线程在，进程就在
	pthread_exit(NULL);
}
