
/*信号量实现 生产者 消费者问题*/

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define NUM 5               

int queue[NUM];                                     //全局数组实现环形队列
sem_t blank_number, product_number;                 //空格子信号量, 产品信号量

static int j = 0;
int flag = 1;

void *producer(void *arg)
{
    int i = 0;

    while (1) {
        sem_wait(&blank_number);                    //生产者将空格子数--,为0则阻塞等待
        queue[i] = rand() % 1000 + 1;               //生产一个产品
        printf("Produce---%d\n", queue[i]);        
		i = (i+1) % NUM;                            //借助下标实现环形
        sem_post(&product_number);                  //将产品数++

        sleep(rand() % 2);
    }
}


void *consumer(void *arg)
{
    int i = 0;

    while (1) {
		if(flag == 1) { 
        sem_wait(&product_number);                  //消费者将产品数--,为0则阻塞等待
        printf("--------Consume:%d\n", queue[j]);
        queue[j] = 0;                               //消费一个产品 
		j = (j+1) % NUM;
        sem_post(&blank_number);                    //消费掉以后,将空格子数++

		flag = 2;
        sleep(rand() % 4);}
    }
}

void *consumer2(void *arg)
{
    int i = 0;

    while (1) {
		if(flag == 2) {
        sem_wait(&product_number);                  //消费者将产品数--,为0则阻塞等待
        printf("--------Consume2:%d\n", queue[j]);
        queue[j] = 0;                               //消费一个产品 
		j = (j+1) % NUM;
        sem_post(&blank_number);                    //消费掉以后,将空格子数++

		flag = 1;
        sleep(rand() % 4); }
    }
}


int main(int argc, char *argv[])
{
    pthread_t pid, cid, ccid;

    sem_init(&blank_number, 0, NUM);                //初始化空格子信号量为5
    sem_init(&product_number, 0, 0);                //产品数为0

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
	pthread_create(&ccid, NULL, consumer2, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    pthread_join(ccid, NULL);

    sem_destroy(&blank_number);
    sem_destroy(&product_number);

    return 0;
}


