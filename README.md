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
- **Head/Tail Queueing:** Keeps both `head` and `tail` pointers so new chunks are appended in constant time.
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

Define how to store the text chunks. The implementation uses two structures:

- `t_gnl_node`: a node containing `char *content` and `next`.
- `t_gnl_list`: a lightweight wrapper containing `head` (first node) and `tail` (last node).

This `head`/`tail` pair is essential:

- `head` marks where line extraction starts.
- `tail` marks where the latest `read()` chunk is stored.
- Appending with `tail->next = new_node` is `O(1)` and avoids traversing the entire list for every new chunk.

#### Step 2: Main Function and Read Loop

`get_next_line` is the conductor. It uses a `static t_gnl_list *list` to remember leftover data between calls.

- It reads from the file and stores chunks in the list until a `\n` is found.
- This is done in `read_and_append`.
- `read_and_append` allocates a buffer of `BUFFER_SIZE + 1`, reads from `fd`, and adds the chunk to the list with `add_to_list`.
- `add_to_list` appends using `tail`, so insertion stays constant-time even for very long lines.

#### Step 3: Detecting the Stop Condition

How does the code know when to stop reading?

- Inside the read loop, it calls `found_newline`.
- `found_newline` checks the `tail` node content for `\n`.
- When it finds `\n`, it returns `1`, telling the loop to stop reading from the file.

### Head/Tail Lifecycle per Call

The list behaves like a queue of chunks:

1. **Append phase:** each `read()` creates one node and links it at `tail`.
2. **Extract phase:** the returned line is assembled by iterating from `head`.
3. **Remainder phase:** after extraction, old nodes are freed and only leftover bytes after `\n` are kept.
4. **Reset phase:** if no remainder exists, both `head` and `tail` become `NULL`.

This model keeps the data flow predictable and makes memory ownership explicit between calls.

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
- Both `head` and `tail` are updated to point to the new remainder node.
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