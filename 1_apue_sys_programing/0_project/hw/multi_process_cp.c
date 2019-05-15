/*****************************************************************************************************************                                                        
                                                阶段综合练习一
实现文件多进程拷贝。
    假设有一个超大文件，需对其完成拷贝工作。为提高效率，可采用多进程并行拷贝的方法来实现。
    
    假设文件大小为len，共有n个进程对该文件进行拷贝。那每个进程拷贝的字节数应为len/n。
    但未必一定能整除，我们可以选择让最后一个进程负责剩余部分拷贝工作，可使用len % (len/n)将剩余部分大小求出。
    为降低实现复杂度，可选用mmap来实现源、目标文件的映射，通过指针操作内存地址，设置每个进程拷贝的起始、结束位置。
    借助MAP_SHARED选项将内存中所做的修改反映到物理磁盘上。
******************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
	int i = 0, j = 0;
	pid_t p;

	if(argc < 4) {
		//传入4个参数：执行程序，源文件，目标文件，进程个数
		printf("u need input: ./a.out src des N \n");
		exit(-1);
	}
	
	//1.指定创建子进程的个数
	int N = *argv[3] - '0';
	printf("Sp Process N: %d \n", N);

	//2.打开源文件
	int fd_src = open(argv[1], O_RDWR);
	if(fd_src < 0) {
		perror("fd_src err: \n");
		exit(-1);
	}

	//3.打开目的文件，不存在则创建
	int fd_des = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if(fd_des < 0) {
		perror("fd_des err: \n");
		exit(-1);
	}

	//4.获取文件大小
	int len = lseek(fd_src, 0, SEEK_END);
	printf("fd_src len: %d \n", len);
		//4.获取文件信息
		//int ret = fstat(fd_src, &buf); 
		// buf.st_size 文件大小

	//5.根据文件大小拓展目标文件
	ftruncate(fd_des, len);

	//6.为源文件创建映射
	char *mem_src = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_src, 0);
	if(mem_src == MAP_FAILED){
		perror("mem_src err: \n");
		exit(-1);
	}
	close(fd_src);
		
	//7.为目标文件创建映射
	char *mem_des = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_des, 0);
	if(mem_des == MAP_FAILED){
		perror("mem_des err: \n");
		exit(-1);
	}
	close(fd_des);

	//拷贝一份留给最后nunmap使用
	char *temp_src = mem_src;
	char *temp_des = mem_des;

	//8.求出每个子进程该拷贝的字节数
	int cp_each_size = len / N;
	int cp_last_size = len % cp_each_size;

	//9.创建N个子进程
	for(i = 0; i < N; ++i) {//父进程出口
		//sleep(i);
		p = fork();
		if(p == -1) {
			perror("fork err");
			exit(-1);
		}
		if(p == 0) {//子进程出口
			break;		
		}	
	}

	//10.子进程完成分块拷贝(注意最后一个拷贝的起始位置)
	if(i == N) {//父进程
		for(j = 0; j < N; ++j) {
			wait(NULL);
		}
		printf("All process have copied finish!!!\n");
	}
	else if(i == (N - 1)) {
		memcpy(temp_des + i * cp_each_size, temp_src + i * cp_each_size, cp_each_size + cp_last_size);
		printf("%d process have copied finish...\n", i);
	}
	else {
		memcpy(temp_des + i * cp_each_size, temp_src + i * cp_each_size, cp_each_size + cp_last_size);				
		printf("%d process have copied finish...\n", i);
	}

	//11.释放映射区
	munmap(mem_src, len);
	munmap(mem_des, len);


	return 0;
}



