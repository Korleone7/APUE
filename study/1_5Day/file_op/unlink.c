#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = open("tempfile", O_CREAT | O_RDWR, 0755);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    int ret = unlink("tempfile");
    if(ret == -1)
    {
        perror("unlink");
        exit(1);
    }

    char buf[512];
    write(fd, "hello\n", 6);
	//reset file pointer
    lseek(fd, 0, SEEK_SET);
	//read file
    int len = read(fd, buf, sizeof(buf));
	//screen print
    //write(STDOUT_FILENO, buf, len);
	write(1, buf, len);

    close(fd);

    return 0;
}
