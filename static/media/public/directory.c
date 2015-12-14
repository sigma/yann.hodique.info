/*  Time-stamp: <10/06/2006 11:54:32 Yann Hodique>  */

/**
 *  @file tp3.c
 *  @date Saturday, June 10, 2006
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
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int lstat(const char *path, struct stat *buf);

int size1(const char* filename) {
    struct stat st;
    if (lstat(filename,&st) == -1) {
        perror("size:");
    }

    if (!S_ISDIR(st.st_mode))
        return st.st_size;
    else return 0;
}

int fsize(const char*);

int size2(const char* filename) {
    struct stat st;
    if (lstat(filename,&st) == -1) {
        perror("size:");
    }

    if (!S_ISDIR(st.st_mode))
        return st.st_size;
    else {
        int res;
        if (!strcmp(filename,".") || !strcmp(filename,".."))
            return 0;
        chdir(filename);
        res = fsize(".");
        chdir("..");
        return res;
    }
}

int size3(const char* filename) {
    struct stat st;
    if (stat(filename,&st) == -1) {
        perror("size:");
    }

    if (!S_ISDIR(st.st_mode))
        return st.st_size;
    else {
        int res;
        if (!strcmp(filename,".") || !strcmp(filename,".."))
            return 0;
        chdir(filename);
        res = fsize(".");
        chdir("..");
        return res;
    }
}

int * visited_links = NULL;
int nb_visited_links = 0;

int visited(int ino) {
    int i;
    for (i = 0; i < nb_visited_links; i++)
        if (visited_links[i] == ino)
            return 1;
    return 0;
}

void visit(int ino) {
    visited_links = realloc(visited_links, ++nb_visited_links);
    visited_links[nb_visited_links - 1] = ino;
}

int size4(const char* filename) {
    struct stat st;
    if (stat(filename,&st) == -1) {
        perror("size:");
    }

    if (!S_ISDIR(st.st_mode)) {
        if (st.st_nlink > 1) {
            if (visited(st.st_ino))
                return 0;
            else
                visit(st.st_ino);
        }
        return st.st_size;
    }
    else {
        int res;
        if (!strcmp(filename,".") || !strcmp(filename,".."))
            return 0;
        chdir(filename);
        res = fsize(".");
        chdir("..");
        return res;
    }
}

void printSize(int s) {
    fprintf(stdout,"Total size: %d\n",s);
}

int fsize(const char* dir) {
    int (*size)(const char*) = size4;

    DIR * d = opendir(dir);
    struct dirent * entry;
    int s = 0;
    while ((entry = readdir(d))) {
        s += size(entry->d_name);
    }
    closedir(d);

    return s;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr,"%s requires exactly one argument", argv[0]);
        exit(1);
    }

    printSize(fsize(argv[1]));

    free(visited_links);
    return 0;
}
