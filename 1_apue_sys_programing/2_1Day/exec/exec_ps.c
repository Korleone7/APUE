#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


// ps aux > ps.out

int main(void)
{
	int fd;

	fd = open("ps.out", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if(fd < 0){
		perror("open ps.out error");
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);

	//exec()只有错误的时候，才返回errno
	//正确的时候，不返回
	execlp("ps", "ps", "ax", NULL);
	perror("exec error");

	//因为exec()正确执行不返回，所以执行完就离开了
	//故而不用写下面close(fd)句话也可以。
	//当进程结束的时候，借助操作系统的隐式回收机制：
	//	1.关闭所有fd
	//	2.释放用户分配的内存
	//	3.内核中PCB仍然存在，保存该进程的退出状态
	//		3.1 正常终止 - 退出值
	//		3.2 异常终止 - 终止信号
	//close(fd);

	return 0;
}
