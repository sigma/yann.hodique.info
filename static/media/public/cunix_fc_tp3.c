#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

int total_lines = 0, total_words = 0, total_chars = 0;

#define BUFFER_SIZE 256
char buffer[BUFFER_SIZE];

void printCount(int lines, int words, int chars, const char* name) {
    printf("%4d %4d %4d %s\n", lines, words, chars, name);
}

void wordCount(int fd, const char * file) {
    int lines = 0, words = 0, chars = 0;
    int i, j;
    int in_spaces = 1;

    while ((i = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (j = 0; j < i; j++) {
            chars++;

            if(in_spaces && !isspace(buffer[j])) {
                words++;
                in_spaces = 0;
            } else if (isspace(buffer[j]))
                in_spaces = 1;

            if (buffer[j] == '\n')
                lines++;
        }
    }

    total_lines += lines;
    total_words += words;
    total_chars += chars;

    printCount(lines, words, chars, file);
}

void printTotal() {
    printCount(total_lines, total_words, total_chars, "total");
}

int main(int argc, char *argv[]) {
    int i, fd;

    if (argc > 1) {

        for (i = 1; i < argc; i++) {
            fd = open(argv[i], O_RDONLY);

            if (fd != -1) {
                wordCount(fd, argv[i]);
                close(fd);
            }
        }
        if (argc > 2)
            printTotal();
    } else {
        wordCount(0, "");
    }
    return 0;
}
