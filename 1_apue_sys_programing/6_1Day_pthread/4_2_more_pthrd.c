#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void *tfn(void *arg)
{
	int i;

	i = (int)arg;
	printf("I'm %dth thread, Thread_ID = %lu\n", i+1, pthread_self());

	return NULL;
}

int main(int argc, char *argv[])
{
	int n = 5, i, ret;
	pthread_t tid;

	if (argc == 2)
		n = atoi(argv[1]);

	for (i = 0; i < n; i++) {
		//将i转换为指针，在tfn中再强转回整形。
		ret = pthread_create(&tid, NULL, tfn, (void *)i);
		if(ret != 0) {
			fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
			exit(-1);
		}
	}

	printf("I am main, and I am not a process, I'm a thread!\n" 
			"main_thread_ID = %lu\n", pthread_self());

	pthread_exit(NULL);
}


