#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

void sys_err(char *str) {
    perror(str);
    exit(1);
}

int main(void) {
    char *mem;
    int len = 0;

    //int fd = open("test.txt", O_RDWR | O_CREAT|O_TRUNC, 0644);
	int fd = open("dict.txt", O_RDWR);
    if (fd < 0) {
	     sys_err("open error");
	}
   	//文件拓展
	//ftruncate(fd, len)会将参数fd指定的文件大小改为参数len指定的大小。
	//if len < fd's prelen : 超过的就会截断删除
	//不能操作只读文件
	if(ftruncate(fd, 10000) == -1)  
	    sys_err("ftruncate err");
	
	//获取文件大小
    len = lseek(fd, 0, SEEK_END);   //获取文件大小,根据文件大小创建映射区
    //write(fd, "e", 1);            //实质性完成文件拓展
    printf("The length of file = %d\n", len);

	//创建test.txt的mmap共享存储映射区
	//offset必须是4KB的整数倍(4096)
    mem = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);
    if (mem == MAP_FAILED)          //出错判断
        sys_err("mmap err");
	
	close(fd);//关闭文件

	//往mmap中写数据
    strcpy(mem, "abc");
    printf("%s\n", mem);

    if (munmap(mem,  len) < 0)//使用完，释放内存(和关闭文件一样)
        sys_err("munmap");
   
    return 0;
}

/*思考：
0. 可以open的时候O_CREAT一个新文件来创建映射区吗?
	true

1. 如果mem++，munmap可否成功？
	false, 如果使用mem，可以用临时变量备份一个再使用

2. 如果open file时O_WRONLY, mmap时PROT参数指定PROT_READ|PROT_WRITE会怎样？
	MAP_SHARED提示没有权限,mmap err: Permission denied				
	MAP_PRIVATE则无所谓

3. 如果文件偏移量为1000会怎样？
	会显示mmap参数失败，因为给映射区分配内存是内核区的mmu，其最小划分单位是4KB	

4. 如果不检测mmap的返回值，会怎样？
	丈二的和尚摸不着头脑	

5. mmap什么情况下会调用失败？
	很多情况

6. 对mem越界操作会怎样？
	同1，mem++的错误

7. 文件描述符先关闭，对mmap映射有没有影响？
	没有，因为映射区一旦创建OK，文件描述符这个句柄就没有用了，
	文件的操作文件的方式已经发生了变化，使用映射区的指针操作文件

*/
