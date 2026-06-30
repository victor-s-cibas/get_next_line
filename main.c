#include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("teste.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	// while (line)
	// {
	// 	printf("%s", line);
	// 	free(line);
	// 	line = get_next_line(fd);
	// }
    printf("%s", line);
	close(fd);
	return (0);
}