/*  Time-stamp: <02/06/2006 11:34:20 Yann Hodique>  */

/**
 *  @file tp2.c
 *  @date Thursday, May 19, 2005
 *  @author Yann Hodique <Yann.Hodique@lifl.fr>
 */

/************************************************************************
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 ************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct {
    int fd;
    /* ... */
} file_t;

void init_file(file_t * file) {
    /* ... */
}

int read_mode(const char *mode) {
    int m = 0;

    switch(*mode) {
        case 'r':
            switch(*(mode + 1)) {
                case '+':
                    m = O_RDWR;
                    break;
                default:
                    m = O_RDONLY;
                    break;
            }
            break;

        case 'w':
            switch(*(mode + 1)) {
                case '+':
                    m = O_RDWR | O_TRUNC;
                    break;
                default:
                    m = O_WRONLY | O_CREAT;
                    break;
            }
            break;

        case 'a':
            switch(*(mode + 1)) {
                case '+':
                    m = O_RDWR | O_CREAT | O_APPEND;
                    break;
                default:
                    m = O_WRONLY | O_CREAT | O_APPEND;
                    break;
            }
            break;

        default:
            break;
    }
    return m;
}

file_t * Fopen(const char *path, const char *mode) {
    file_t * file = NULL;
    int fd;

    if((fd = open(path,read_mode(mode),S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) == -1) {
        perror("Fopen");
        return file;
    }

    file = malloc(sizeof(file_t));
    init_file(file);
    file->fd = fd;

    return file;
}

int Fclose(file_t *fp) {
    int fd = fp->fd;
    free(fp);
    return close(fd) || EOF;
}

size_t Fread(void *ptr, size_t size, size_t mmem, file_t *stream) {
    /* ... */
    return 0;
}

size_t Fwrite(const void *ptr, size_t size, size_t mmem, file_t *stream) {
    /* ... */
    return 0;
}

int Feof(file_t *stream) {
    /* ... */
    return 0;
}

int Ferror(file_t *stream) {
    /* ... */
    return 0;
}

int main(int argc, char *argv[]) {
    char buffer[8];
    file_t * f = Fopen("plop","w");
    Fwrite("okokok\n",sizeof(char),7,f);
    Fclose(f);

    f = Fopen("plop","r");
    Fread(buffer,sizeof(char),8,f);
    Fclose(f);

    f = Fopen("plop","a");
    Fwrite("okokok\n",sizeof(char),7,f);
    Fclose(f);

    printf("%s",buffer);
    return 0;
}
