#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void err_thread(int ret, char *str)
{
    if (ret != 0) {
        fprintf(stderr, "%s:%s\n", str, strerror(ret));
        pthread_exit(NULL);
    }
}

/*线程之间共享资源stdout*/
pthread_mutex_t mutex;      //定义锁

void *tfn(void *arg)
{
    srand(time(NULL));

    while (1) {
        pthread_mutex_lock(&mutex);
        printf("SonThread:		hello ");
        sleep(rand() % 3);	/*模拟长时间操作共享资源，导致cpu易主，产生与时间有关的错误*/
        printf("world\n\n");
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3);
    }

    return NULL;
}

int main(void)
{
    int flg = 5;
    pthread_t tid;
    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);  // mutex==1  创建锁
    pthread_create(&tid, NULL, tfn, NULL);
    
    while (flg--) {
        pthread_mutex_lock(&mutex);//mutex-- 加锁
        
        printf("FatherThread:		HELLO ");
        sleep(rand() % 3);
        printf("WORLD\n\n");
        
        pthread_mutex_unlock(&mutex);//mutex++ 解锁

        sleep(rand() % 3);
    }
    
    pthread_cancel(tid);    //将子线程杀死，子线程中自带取消点，因此只会打印一半
    pthread_join(tid, NULL);//回收子线程

    pthread_mutex_destroy(&mutex);//销毁锁  

    return 0;//main中的return可以将整个进程退出
}



