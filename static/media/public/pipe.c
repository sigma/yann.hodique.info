/*  Time-stamp: <15/06/2005 18:05:11 Yann Hodique>  */

/**
 *  @file pipe.c
 *  @date Wednesday, June 15, 2005
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

char ** ARGV;

void createProcess(int number, int * p) {
    char *args[2];
    args[0] = ARGV[1+number];
    args[1] = NULL;

    close(p[number]);
    close(1-number);
    dup(p[1-number]);
    close(p[1-number]);

    execvp(args[0], args);
}

int main(int argc, char *argv[]) {
    int p[2];
    int pid1, pid2;

    ARGV = argv;

    if(argc != 3) {
        fprintf(stderr, "Wrong argument number\n");
        exit(1);
    }

    if(pipe(p) == -1) {
        perror("Pipe");
        exit(1);
    }

    if((pid1 = fork()) == -1) {
        perror("Fork");
        exit(1);
    }

    if(pid1) {
        if((pid2 = fork()) == -1) {
            perror("Fork");
            exit(1);
        }

        if(pid2) {
            close(p[0]);
            close(p[1]);
            wait(NULL);
            wait(NULL);
        } else {
            createProcess(0,p);
        }
    } else {
        createProcess(1,p);
    }

    return 0;
}
