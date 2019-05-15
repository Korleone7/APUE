#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>

#define T_NUM 5 //线程数
#define ITEMS 50 //

void err_sys(void *str)
{
	perror(str);
	exit(1);
}

void err_usr(char *str)
{
	fputs(str, stderr);
	exit(1);
}

//数据拷贝结构体
typedef struct{
	int off, size, t_no;
}arg_t;

char *s, *d;
int *done;
int n = T_NUM;


/**************************
 * 数据拷贝线程
 *
 * arg{off, size, t_no;}
 *
***************************/
void *tfn(void *arg)
{
	arg_t *arg_p; 
	int i;
	char *p, *q;

	arg_p = (arg_t *)arg;
	p = s + arg_p->off;
	q = d + arg_p->off;

	for(i = 0; i < arg_p->size; i++) {
		/* 逗号表达式的使用技巧*/
		*q++ = *p++;
		done[arg_p->t_no]++;//将第t_no个线程 要拷贝的字节数累加
		usleep(3);
	}
	//printf("done %d", done[arg_p->t_no]);

	return NULL;
}

/****************************
 * 进度显示线程
 *
 * arg = {statbuf.st_size} 文件大小
 ****************************/
void *display(void *arg)
{
	int size, interval, draw, sum, i, j;

	size = (int)arg;
	interval = size / (ITEMS - 1);
	draw = 0;

	while(draw < ITEMS){
		for(i = 0, sum = 0; i < n; i++)
			sum += done[i];

		j = sum / interval + 1;

		for(; j > draw; draw++) {
			putchar('='); 
			//程序的输出内容一般不会立即输出，而是在程序结束后再输出。
			//fflush(stdout)会强制每次printf()都立即显示在标准输出设备上。
			fflush(stdout);
		}
	}

	putchar('\n');

	return NULL;
}

int main(int argc, char *argv[])
{
	int src, dst, i, len, off;
	struct stat statbuf;
	pthread_t *tid;
	arg_t *arr;

	if(argc != 3 && argc != 4)
		err_usr("usage : cp src dst [thread_no]\n");
	if(argc == 4)
		n = atoi(argv[3]);

	//只读方式打开源文件
	src = open(argv[1], O_RDONLY);
	if(src == -1)
		err_sys("fail to open");

	//读写方式打开目标文件，不存在则创建，存在则截断
	dst = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if(dst == -1)
		err_sys("fail to open");

	//获取文件状态statbuf
	if(fstat(src, &statbuf) == -1)
		err_sys("fail to stat");

	//创建同样的大小文件
	//重新设置打开文件的文件指针偏移量
	lseek(dst, statbuf.st_size - 1, SEEK_SET);
	write(dst, "a", 1);                          //IO操作拓展文件大小,也可以使用truncate

	//源文件mmap映射区 s
	s = (char *)mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, src, 0);
	if(s == MAP_FAILED)
		err_sys("fail to mmap");

	//目标文件mmap映射区 d
	d = (char *)mmap(NULL, statbuf.st_size, PROT_WRITE , MAP_SHARED, dst, 0);
	if(d == MAP_FAILED)
		err_sys("fail to mmap");

	//
	close(src); 
	close(dst);


	//动态创建存储变量
	//线程数组 pthread_t tid[n+1];
	tid = (pthread_t *)malloc(sizeof(pthread_t) * (n + 1));
	if(tid == NULL)
		err_sys("fail to malloc");

    //每个线程完成任务字节数 int done[n] 
	done = (int *)calloc(sizeof(int), n);
	if(done == NULL)
		err_sys("fail to malloc");

    //每个线程的任务 arr[n]
	arr = (arg_t *)malloc(sizeof(arg_t) * n);
	if(arr == NULL)
		err_sys("fail to malloc");

    //构建线程任务数组，分配任务
	len = statbuf.st_size / n;
	off = 0;
	for(i = 0; i < n; i++, off += len) {
		arr[i].off = off;
		arr[i].size = len;
		arr[i].t_no = i; 
	}
	arr[n - 1].size += (statbuf.st_size % n);
    
    //创建 拷贝任务线程
	for(i = 0; i < n; i++)
		pthread_create(&tid[i], NULL, tfn, (void *)&arr[i]);

    //创建 进度线程
	pthread_create(&tid[n], NULL, display, (void *)statbuf.st_size);

	for(i = 0; i < n + 1; i++)
		pthread_join(tid[i], NULL);
#if 1 
	munmap(s, statbuf.st_size);
	munmap(d, statbuf.st_size);
#endif
	free(tid); free(done); free(arr);

	return 0;
}


