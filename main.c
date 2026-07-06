#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "get_next_line.h"

int main(void)
{
    int fd1 = open("test.txt", O_RDONLY);
	int fd2 = open("test2.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd1)) != NULL)
    {
        printf("FD 1: %s \n", line);
		free(line);
    }
	printf("\nAgora o teste do segundo file descriptor: \n\n");
	while ((line = get_next_line(fd2)) != NULL)
	{
		printf("FD 2: %s \n", line);
		free(line);
	}
    close(fd1);
	close(fd2);
    return (0);
}