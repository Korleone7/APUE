#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

void err_int(int ret, const char *err)
{
    if (ret == -1) {
        perror(err);
        exit(1);
    }

    return ;
}

void err_str(char *ret, const char *err)
{
    if (ret == MAP_FAILED) {
        perror(err);
        exit(1);
    }
}

int main(int argc, char *argv[])
{   
    int fd_src, fd_dst, ret, len, i, n;
    char *mp_src, *mp_dst, *tmp_srcp, *tmp_dstp;
    pid_t pid;
    struct stat sbuf;

    if (argc < 3 || argc > 4) {
        printf("Enter like this please: ./a.out file_src file_dst [process number]\n");
        exit(1);
    } else if (argc == 3) {
        n = 5;                  //用户未指定,默认创建5个子进程
    } else if (argc == 4) {
        n = atoi(argv[3]);
    }

	//只读的方式打开源文件
    fd_src = open(argv[1], O_RDONLY);
    err_int(fd_src, "open dict.txt err");

	//读写方式打开目标文件，不存在则创建，已存在则截断
    fd_dst = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0664);
    err_int(fd_dst, "open dict.cp err");

	//获取文件状态
    ret = fstat(fd_src, &sbuf);
    err_int(ret, "fstat err");

	//文件长度小于进程个数
    len = sbuf.st_size;
    if (len < n)               
		n = len;

	//truncate目标文件的长度
    ret = ftruncate(fd_dst, len);
    err_int(ret, "truncate fd_dst err");

   	//建立源文件的mmap映射区
	mp_src = (char *)mmap(NULL, len, PROT_READ, MAP_SHARED, fd_src, 0);
    err_str(mp_src, "mmap src err");

	//建立目标文件的mmap映射区
    mp_dst = (char *)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dst, 0);
    err_str(mp_dst, "mmap dst err");

	//备份映射区地址
    tmp_dstp = mp_dst;
    tmp_srcp = mp_src;

	//进程拷贝预处理
    int bs = len / n;    //每个子进程应该拷贝的字节数
    int mod = len % bs;  //求出均分后余下的字节数,让最后一个子进程处理

    for (i = 0; i < n; i++) {
		sleep(1);
        if ((pid = fork()) == 0) {
            break;
        }
    }

	//多进程拷贝
    if (n == i) {               //父进程回收
        for (i = 0; i < n; i++)
            wait(NULL);

		printf("\ncopy finished\n");
    } else if (i == (n-1)){     //最后一个子进程,它多处理均分后剩余几个字节
        memcpy(tmp_dstp + i*bs, tmp_srcp + i * bs, bs + mod); 
		//sleep(1);
		printf("="); 
    } else if (i == 0) {        //第一个子进程
        memcpy(tmp_dstp, tmp_srcp, bs); 
		//sleep(1);
		printf("=");
    } else {                    //其他子进程
        memcpy(tmp_dstp + i * bs + 1, tmp_srcp + i * bs + 1, bs);
		//sleep(1);
		printf("=");
    }

	//释放mmap映射区
    munmap(mp_src, len);
    munmap(mp_dst, len);

    return 0;
}
