*This project has been created as part of the 42 curriculum by vicdos-s.*

[![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=for-the-badge&logo=42)](https://github.com/42School/norminette)
[![C](https://img.shields.io/badge/language-C-blue.svg?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))

---

## Description

This project consists of implementing a highly optimized C function named `get_next_line` that returns one line at a time from a file descriptor.

The main goal is to read a text file sequentially across repeated calls, one line per invocation. This project introduces the concept of static variables in C, dynamic memory management, and efficient input handling without relying on heavy or unsafe string operations.

### Key Features

- **Universal Reading:** Reads from any valid file descriptor, including regular files and standard input.
- **Dynamic Scaling:** Supports arbitrary `BUFFER_SIZE` values defined at compile time (from `1` to very large values).
- **Engineered Data Structure:** Uses a singly linked list to store chunks of data dynamically.
- **Optimized Memory:** Avoids the overhead of repeated array resizing and string concatenation.
- **Leak-Free:** Handles EOF and read errors safely while remaining memory-safe.

---

## Instructions

To use this project, include the header in your source code and compile the implementation files together with your program.

### Mandatory Part

Compile your project pointing to the standard files:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl_test
```

### Usage Example

A typical pattern is to call `get_next_line` inside a loop until it returns `NULL`:

```c
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
```

---

## The Core Engine: Linked Lists in GNL

While many implementations of `get_next_line` rely on repeated string concatenation, this project uses a more optimized approach: a singly linked list.

### The Big Problem with `strjoin`

If you build `get_next_line` using the classic string concatenation approach, every time you read a new `BUFFER_SIZE` chunk, you need to do a `strjoin`. That means:

1. Allocate a new string of size `old_length + buffer_length`.
2. Copy the entire old string into the new memory.
3. Copy the new buffer into the new memory.
4. Free the old string.

If the input line is 10,000 characters long and `BUFFER_SIZE` is `1`, you end up allocating and copying memory 10,000 times. The program keeps re-reading and re-copying the beginning of the line over and over, which makes the algorithm extremely slow for large files.

### Why the Linked List is Better

With the linked list approach, you allocate a new node for each read and append it to the end of the list. You do not recopy the data that has already been read. The heavy operation (`malloc` plus the transfer of characters) happens only once at the end, when you already know the exact length of the line.

### Algorithm Justification

This implementation was chosen because it separates read buffering from line construction:

- It avoids repeated string resizing and copying on every read.
- It keeps memory allocations small and incremental while reading.
- It only performs the final line assembly once, using the exact required size.
- It preserves the remainder after `\n` cleanly between calls using a static list.

### Step-by-Step: Building GNL with a List

If we break the problem into logical steps, the code follows this exact structure:

#### Step 1: Data Structure

Define how to store the text chunks. We use a singly linked list node:

- `t_gnl_list`: contains `char *content` for the read text and `struct s_gnl_list *next` for the next chunk.

#### Step 2: Main Function and Read Loop

`get_next_line` is the conductor. It uses a `static t_gnl_list *list` to remember leftover data between calls.

- It reads from the file and stores chunks in the list until a `\n` is found.
- This is done in `read_and_append`.
- `read_and_append` allocates a buffer of `BUFFER_SIZE + 1`, reads from `fd`, and adds the chunk to the list with `add_to_list`.
- `add_to_list` creates a new node and appends it to the end of the static list.

#### Step 3: Detecting the Stop Condition

How does the code know when to stop reading?

- Inside the read loop, it calls `found_newline`.
- `found_newline` walks the list and checks each character for `\n`.
- When it finds `\n`, it returns `1`, telling the loop to stop reading from the file.

#### Step 4: Extracting the Final Line

Now the list contains all the chunks needed for the line, and the last chunk may contain the `\n` plus extra characters for the next line.

- `extract_line` creates the string that will be returned to the caller.
- It first calls `count_len` to compute exactly how many characters exist up to `\n` (or end of input).
- Then it performs a single `malloc` with the exact required size.
- `copy_line` fills the new string by iterating through the list and copying characters until the `\n` is copied, then terminates with `\0`.

#### Step 5: Cleanup and Remainder Handling

After extracting the line, the characters read after `\n` must be kept for the next call to `get_next_line`.

- `update_list` walks to the node containing the newline and advances the read index past it.
- `alloc_remainder` creates a new node containing only the characters after `\n`.
- `free_list` clears the old list that has already been consumed.
- The static pointer is updated to point to the new remainder node.
- If the remainder is empty, that node is freed as well.

This design gives you fine control over memory and preserves the leftover data without repeated concatenation.

---

## Resources & AI Usage

- The C `read()` system call documentation and file I/O references.
- [Chapter on linked lists (IME-USP)](https://www.ime.usp.br/~pf/algoritmos/aulas/lista.html): didactic content on linked-list structures, search, and insertion.
- [Former student's guide on GNL](https://yannick.eu/gnl/): a brief explanation of the project with an intuitive diagram.
- [Oceano's video about the project](https://www.youtube.com/watch?v=8E9siq7apUU): a visual explanation of the project.

### AI Usage

- **Claude:** Used as a Socratic reasoning validator. Its role was limited to: (1) running prerequisite diagnostics before new topics, (2) pointing out gaps or errors in my own Feynman-style explanations — without supplying the explanation itself — and (3) generating practice/review material (flashcards, exercise lists) after I demonstrated understanding.
- **Google Gemini:** Used occasionally for technical troubleshooting and documentation refinement. Its role was limited to: (1) interpreting and diagnosing complex Valgrind error messages during debugging, and (2) assisting with the formatting, translation, and English localization of this README.
- **GitHub Copilot (embedded VS Code agent):** Used for local review of the README content directly inside Visual Studio Code. Its role was limited to suggesting wording improvements, clarifying phrasing, and checking documentation structure without modifying the implementation details.

---

## Links
[![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/victor-s-cibas/)
[![linkedin](https://img.shields.io/badge/linkedin-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/victorcibas)

---


## Versão em Português

*Este projeto foi criado como parte do currículo 42 por vicdos-s.*

[![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=for-the-badge&logo=42)](https://github.com/42School/norminette)
[![C](https://img.shields.io/badge/language-C-blue.svg?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))

---

## Descrição

Este projeto consiste em implementar uma função em C chamada `get_next_line`, que retorna uma linha por vez a partir de um descritor de arquivo.

O objetivo principal é ler um arquivo de texto sequencialmente em chamadas repetidas, uma linha por invocação. Este projeto introduz o uso de variáveis estáticas em C, gerenciamento dinâmico de memória e leitura eficiente de entrada sem depender de operações de string pesadas ou inseguras.

### Principais Características

- **Leitura Universal:** Lê de qualquer descritor de arquivo válido, incluindo arquivos normais e entrada padrão.
- **Escalabilidade Dinâmica:** Suporta valores arbitrários de `BUFFER_SIZE` definidos na compilação (de `1` até valores grandes).
- **Estrutura de Dados Projetada:** Usa uma lista ligada simples para armazenar pedaços de dados dinamicamente.
- **Memória Otimizada:** Evita o custo de redimensionamento repetido de arrays e concatenação de strings.
- **Sem Vazamentos:** Trata EOF e erros de leitura com segurança, mantendo a segurança de memória.

---

## Instruções

Para usar este projeto, inclua o cabeçalho em seu código e compile os arquivos de implementação junto com o seu programa.

### Parte Obrigatória

Compile o projeto apontando para os arquivos padrão:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl_test
```

### Exemplo de Uso

Um padrão típico é chamar `get_next_line` dentro de um loop até que ela retorne `NULL`:

```c
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
```

---

## O Motor Principal: Listas Ligadas no GNL

Enquanto muitas implementações de `get_next_line` dependem de concatenação contínua de strings, este projeto usa uma abordagem mais otimizada: uma lista ligada simples.

### O Grande Problema do `strjoin`

Se você construir `get_next_line` usando a abordagem clássica de concatenação de strings, toda vez que você lê um novo pedaço de `BUFFER_SIZE`, precisa fazer um `strjoin`. Isso significa:

1. Alocar uma nova string com tamanho `tamanho_antigo + tamanho_buffer`.
2. Copiar toda a string antiga para a nova memória.
3. Copiar o novo buffer para a nova memória.
4. Liberar a string antiga.

Se a linha de entrada tiver 10.000 caracteres e o `BUFFER_SIZE` for `1`, você acaba alocando e copiando memória 10.000 vezes. O programa volta a copiar o início da linha repetidamente, o que torna o algoritmo extremamente lento para arquivos grandes.

### Por que Lista Ligada? (Ou Lista Encadeada / Linked List)

Com a abordagem da lista ligada, você aloca um novo nó para cada leitura e o adiciona ao final da lista. Você não recopia os dados que já foram lidos. A operação pesada (`malloc` e a transferência dos caracteres) acontece apenas uma vez no final, quando você já sabe o tamanho exato da linha.

### Justificativa do Algoritmo

Esta implementação foi escolhida porque separa o buffer de leitura da construção da linha:

- Evita redimensionamento e cópia repetida de strings a cada leitura.
- Mantém as alocações de memória pequenas e incrementais durante a leitura.
- Executa a montagem final da linha apenas uma vez, usando exatamente o tamanho necessário.
- Preserva o restante após o `\n` de forma limpa entre as chamadas usando uma lista estática.

### Passo a Passo: Construindo o GNL com Lista

Se dividirmos o problema em etapas lógicas, o código segue exatamente esta estrutura:

#### Passo 1: Estrutura de Dados

Defina como guardar os pedaços de texto. Usamos um nó de lista ligada simples:

- `t_gnl_list`: contém `char *content` para o texto lido e `struct s_gnl_list *next` para o próximo pedaço.

#### Passo 2: Função Principal e Loop de Leitura

`get_next_line` é o maestro. Ela usa `static t_gnl_list *list` para lembrar os dados que sobram entre as chamadas.

- Ela lê do arquivo e armazena pedaços na lista até encontrar um `\n`.
- Isso é feito em `read_and_append`.
- `read_and_append` aloca um buffer de `BUFFER_SIZE + 1`, lê do `fd` e adiciona o pedaço à lista com `add_to_list`.
- `add_to_list` cria um novo nó e o anexa ao final da lista estática.

#### Passo 3: Detectando a Condição de Parada

Como o código sabe quando deve parar de ler?

- Dentro do loop de leitura, chama-se `found_newline`.
- `found_newline` percorre a lista e verifica cada caractere em busca de `\n`.
- Quando encontra `\n`, retorna `1`, avisando o loop para parar de ler do arquivo.

#### Passo 4: Extraindo a Linha Final

Agora a lista contém todos os pedaços necessários para a linha, e o último pedaço pode conter o `\n` e talvez caracteres a mais para a próxima linha.

- `extract_line` cria a string que será retornada ao usuário.
- Primeiro, chama `count_len` para calcular exatamente quantos caracteres existem até o `\n` (ou até o fim).
- Em seguida, faz um único `malloc` com o tamanho exato necessário.
- `copy_line` preenche a nova string iterando pelos nós da lista e copiando caracteres até encontrar o `\n`, que também é copiado, e finaliza com `\0`.

#### Passo 5: Limpeza e Tratamento do Resto

Depois de extrair a linha, os caracteres lidos após o `\n` precisam ser preservados para a próxima chamada de `get_next_line`.

- `update_list` percorre até o nó que contém a nova linha e avança o índice de leitura além do `\n`.
- `alloc_remainder` cria um novo nó contendo apenas os caracteres que sobraram após o `\n`.
- `free_list` libera a lista antiga que já foi consumida.
- O ponteiro estático passa a apontar para o novo nó de resto.
- Se o resto estiver vazio, esse nó também é liberado.

Esse design permite controlar bem a memória e preservar os dados leftovers sem usar concatenação repetida.

---

## Recursos & Uso de IA

- Documentação da chamada de sistema `read()` em C

- [Artigo sobre listas encadeadas (IME-USP)](https://www.ime.usp.br/~pf/algoritmos/aulas/lista.html): conteúdo didático sobre estruturas, busca e inserção em listas ligadas.
- [Guia de um ex-aluno sobre GNL](https://yannick.eu/gnl/): uma breve explicação do projeto com um diagrama intuitivo.
- [Vídeo do Oceano sobre o projeto](https://www.youtube.com/watch?v=8E9siq7apUU): uma explicação visual do projeto.

### Uso de IA

- **Claude:** Utilizado como validador socrático de raciocínio. Sua função limitou-se a: (1) realizar diagnósticos de pré-requisitos antes de novos tópicos, (2) apontar lacunas ou erros nas minhas próprias explicações ao estilo Feynman — sem fornecer a explicação em si — e (3) gerar material de prática/revisão (flashcards, listas de exercícios) após eu demonstrar compreensão.
- **Google Gemini:** Utilizado ocasionalmente para resolução de problemas técnicos e refinamento de documentação. Sua função limitou-se a: (1) interpretar e diagnosticar mensagens de erro complexas do Valgrind durante a depuração e (2) auxiliar na formatação, tradução e localização para o inglês deste README.
- **GitHub Copilot (agente embutido do VS Code):** Utilizado para revisão local do conteúdo do README diretamente dentro do Visual Studio Code. Sua função limitou-se a sugerir melhorias de redação, esclarecer frases e verificar a estrutura da documentação, sem modificar os detalhes de implementação.

## Links
[![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/victor-s-cibas/)
[![linkedin](https://img.shields.io/badge/linkedin-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/victorcibas)
