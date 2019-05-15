/*借助条件变量模拟 生产者-消费者 问题*/
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

/*链表作为公享数据,需被互斥量保护*/
struct msg {
    struct msg *next;
    int num;
};

struct msg *head;
struct msg *mp;

/* 静态初始化 一个条件变量 和 一个互斥量*/
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *p)
{
    for (;;) {
        pthread_mutex_lock(&lock);
        while (head == NULL) {           //头指针为空,说明没有节点    可以为if吗
   			//1.阻塞等待条件变量cond满足
			//2.释放已经掌握的互斥锁(解锁互斥变量)  unlock
			//3.当被唤醒，该函数返回时，解除阻塞且重新申请获取互斥锁 lock
            pthread_cond_wait(&has_product, &lock);
        }
        mp = head;      
        head = mp->next;    //模拟消费掉一个产品

        printf("-Consume ---%d\n", mp->num);
        free(mp);
        mp = NULL;
        pthread_mutex_unlock(&lock);

        sleep(rand() % 2);
    }
}

void *producer(void *p)
{
    for (;;) {
        pthread_mutex_lock(&lock);//上锁

		mp = malloc(sizeof(struct msg));
        mp->num = rand() % 1000 + 1;        //模拟生产一个产品
        printf("-Produce ---%d\n", mp->num);

        mp->next = head;
        head = mp;
        pthread_mutex_unlock(&lock);//解锁

        pthread_cond_signal(&has_product);  //将等待在该条件变量上的一个线程唤醒
        sleep(rand() % 2);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pid, cid;
    srand(time(NULL));
	
	//time_t cur = time(NULL);
	//printf("====%ld \n\n", cur);

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    return 0;
}
