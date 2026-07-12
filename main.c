#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "get_next_line.h"

int main(void)
{
    int fd = open("test.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}

// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h> // Necessário para o free()
// #include <unistd.h>
// #include "get_next_line.h"

// int main(void)
// {
//     int     fd1, fd2, fd3;
//     char    *line1, *line2, *line3;
//     int     continuar_lendo;

//     // Abra três arquivos diferentes (crie-os antes de rodar!)
//     fd1 = open("test1.txt", O_RDONLY);
//     fd2 = open("test2.txt", O_RDONLY);
//     fd3 = open("test3.txt", O_RDONLY);

//     // Proteção básica para garantir que os arquivos existem
//     if (fd1 < 0 || fd2 < 0 || fd3 < 0)
//     {
//         printf("Erro ao abrir os arquivos.\n");
//         return (1);
//     }

//         if (line2)
//         {
//             printf("FD 2: %s", line2);
//             free(line2);
//             continuar_lendo = 1;
//         }

//         // Lê do FD 3
//         line3 = get_next_line(fd3);
//         if (line3)
//         {
//             printf("FD 3: %s", line3);
//             free(line3);
//             continuar_lendo = 1;
//         }
//     }

//     // Fecha todos os descritores no final
//     close(fd1);
//     close(fd2);
//     close(fd3);
    
//     return (0);
// }continuar_lendo = 1;
//     while (continuar_lendo)
//     {
//         // Começamos assumindo que tudo acabou, a menos que uma linha seja lida
//         continuar_lendo = 0; 

//         // Lê do FD 1
//         line1 = get_next_line(fd1);
//         if (line1)
//         {
//             printf("FD 1: %s", line1);
//             free(line1);
//             continuar_lendo = 1; // Achou linha, então o loop deve rodar de novo
//         }

//         // Lê do FD 2 logo em seguida
//         line2 = get_next_line(fd2);
//         if (line2)
//         {
//             printf("FD 2: %s", line2);
//             free(line2);
//             continuar_lendo = 1;
//         }

//         // Lê do FD 3
//         line3 = get_next_line(fd3);
//         if (line3)
//         {
//             printf("FD 3: %s", line3);
//             free(line3);
//             continuar_lendo = 1;
//         }
//     }

//     // Fecha todos os descritores no final
//     close(fd1);
//     close(fd2);
//     close(fd3);
    
//     return (0);
// }