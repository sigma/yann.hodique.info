/*  Time-stamp: <15/06/2005 19:13:10 Yann Hodique>  */

/**
 *  @file adder.c
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
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void sendData(int out) {
    int data[] = {1, 2, 3, 4, 5, 6};

    if(write(out, data, sizeof(data)) == -1) {
        perror("write");
        exit(1);
    }
}

void computeResults(int in, int out) {
    int ops[2];
    int nb;

    while((nb = read(in, ops, sizeof(ops))) > 0) {
        int res = ops[0] + ops[1];
        write(out, &res, sizeof(res));
    }
    if(nb == -1) {
        perror("read");
        exit(1);
    }
}

void displayResults(int in) {
    int res;
    int nb;

    while((nb = read(in, &res, sizeof(res))) > 0) {
        printf("%d\n", res);
    }
    if(nb == -1) {
        perror("read");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int pid;
    int p1[2], p2[2];

    if(pipe(p1) == -1 ||
       pipe(p2) == -1) {
        perror("Pipe");
        exit(1);
    }

    if((pid = fork()) == -1) {
        perror("Fork");
        exit(1);
    }

    if(pid) {
        close(p1[0]);
        close(p2[1]);

        sendData(p1[1]);
        close(p1[1]);

        displayResults(p2[0]);
        close(p2[0]);

        wait(NULL);
    } else {
        close(p2[0]);
        close(p1[1]);

        computeResults(p1[0], p2[1]);

        close(p1[0]);
        close(p2[1]);
    }
    return 0;
}
