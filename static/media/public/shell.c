/*  Time-stamp: <24/06/2005 01:05:42 Yann Hodique>  */

/**
 *  @file shell.c
 *  @date Thursday, June 23, 2005
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
#include <string.h>
#include <sys/wait.h>

#include "command_line.h"

/*
 * Structure pour une commande interne du shell
 * name : nom de la commande
 * function : fonction à appeler
 */
typedef struct {
    char * name;
    void (*function)(char ** args);
} s_builtin;

/*
 * Commande cd
 */
void cdCommand(char ** args) {
    chdir(args[1]);
}

/*
 * Commande exit
 */
void exitCommand(char ** args) {
    exit(0);
}

/*
 * Ensemble des builtins existants
 */
s_builtin builtins[] = {{"cd", cdCommand},
                        {"exit", exitCommand}};

extern char *get_current_dir_name(void);

/*
 * Affichage d'un prompt contenant le nom du répertoire courant
 */
void prompt() {
    fprintf(stdout,"[%s]$ ", get_current_dir_name());
    fflush(stdout);
}

/* Sauvegarde les IO standards et substitue in et out */
void saveIO(int in, int out, int * sin, int * sout) {
    *sin = dup(0);
    *sout = dup(1);
    close(0);
    close(1);
    dup(in);
    dup(out);
    close(in);
    close(out);
}

/* Restaure les IO standards */
void restoreIO(int sin, int sout) {
    close(0);
    close(1);
    dup(sin);
    dup(sout);
    close(sin);
    close(sout);
}

/* Lancement d'un processus avec les arguments args, et IO sur in et out */
void launchProcess(char ** args, int in, int out) {
    int i;
    for(i = 0; i < sizeof(builtins)/sizeof(s_builtin); i++) {
        if(!strcmp(args[0],builtins[i].name)) {
            /* La commande à exécuter est un builtin */
            int sin, sout;

            saveIO(in, out, &sin, &sout);
            (builtins[i].function)(args);
            restoreIO(sin, sout);

            return;
        }
    }

    /* vrai processus à créer */
    if(!fork()) {
        /* On corrige stdin si nécessaire */
        if(in != 0) {
            close(0);
            dup(in);
            close(in);
        }

        /* On corrige stdout si nécessaire */
        if(out != 1) {
            close(1);
            dup(out);
            close(out);
        }

        execvp(args[0], args);
        exit(1); /* pas un vrai programme => quit */
    }
}

/* un pas d'exécution du shell */
void step() {
    int i;
    int flag, nb;
    int p[2];
    int in = 0;

    /* lecture de la ligne de commande et découpage */
    char *** t = commandLine(&flag, &nb);
    if(t == NULL) return;

    /* tous les processus ayant un pipe à leur droite */
    for(i = 0; i < nb-1; i++) {
        pipe(p); /* création du pipe */
        launchProcess(t[i], in, p[1]); /* lancement du processus */

        close(p[1]);
        in = p[0]; /* récupération de l'autre extrémité du pipe pour tour
                    * suivant */
    }
    launchProcess(t[i], in, 1); /* dernier processus doit afficher sur la
                                 * console */

    if(!flag) /* ne pas attendre le retour des processus */
        for(i = 0; i < nb; i++)
            wait(NULL);

    freeLine(t);
}

int main(int argc, char *argv[]) {

    while(1) {
        prompt();
        step();
    }

    return 0;
}
