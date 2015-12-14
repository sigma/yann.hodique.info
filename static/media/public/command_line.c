#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define STEP 5

void displayLine (char ***t) {
   int i = 0, j;
   while (t[i] != NULL) {
      printf ("---> command %d\n", i);
      j = 0;
      while (t[i][j] != NULL) {
         printf("      argv[%d] = %s\n", j,t[i][j]);
         j++;
      }
      i++;
   }
}

void freeLine (char ***t) {
   int i = 0, j;

   while (t[i] != NULL) {
      j = 0;
      while (t[i][j] != NULL) {
         free(t[i][j]);
         j++;
      }
      free(t[i]);
      i++;
   }
   free(t);
}

char * getCommandLine() {
   int t = STEP, i = 0;
   char *line = (char*) malloc (sizeof(char) * t);
   char *tmp;
   int n = 0;
   while ((n = read(0, line+i, sizeof(char)*STEP)) > 0) {
      if (n == STEP && *(line+i+STEP-1) != '\n') {
         t = t + STEP;
         tmp = (char *) realloc (line, sizeof(char) * t);
         if (tmp == NULL) {
            perror("realloc");
            free(line);
            return NULL;
         }
         line = tmp;
         i = i + n;
      }
      else {
         *(line+i+n-1) = '\0';
         return line;
      }
   }
   free(line);
   if (n == 0) exit(0);
   if (n == -1) {
      return NULL;
   }
   exit(0);
}

int separator (int c) {
   return c == '|' || c == '&' || c == '\0';
}

char *** commandLine (int * flag, int * nb) {
   char * line = getCommandLine();
   char *** tab, ***tmp_tab, **tmp_mot, *tmp_car;
   int i_tab = 0, t_tab = STEP, t_mot = STEP, t_car = STEP;
   int i = 0, i_mot, i_car;

   if (line == NULL) return NULL;
   while (line[i] != '\0' && (line[i] == ' ' || line[i] == '\t')) i++;
   if (line[i] == '\0') {free(line); return NULL;}
   tab = (char ***) malloc (STEP * sizeof(char**));
   if (tab == NULL ) return NULL;
   while (line [i] != '\0' && line [i] != '&') {
      if (i_tab == t_tab -1) {
         t_tab += STEP;
         tmp_tab = (char ***) realloc (tab, sizeof(char**)*t_tab);
         if (tmp_tab == NULL) {
            tab[i_tab] = NULL;
            free(line);
            *flag = -1;
            return tab;
	 }
         tab = tmp_tab;
      }
      if ((tab[i_tab] = (char**) malloc (sizeof(char *) * STEP)) == NULL) {
         *flag = -1;
         free(line);
         return tab;
      }

      t_mot = STEP;
      i_mot = 0;
      while (!separator(line [i])) {
         if (i_mot == t_mot -1) {
            t_mot = t_mot+STEP;
	    /****/
            tmp_mot =(char **) realloc (tab[i_tab], sizeof(char*) * t_mot);
            if (tmp_mot == NULL) {
               tab[i_tab][i_mot] = NULL;
               tab[i_tab+1] = NULL;
               free(line);
               *flag = -1;
               return tab;
	    }
            tab[i_tab] = tmp_mot;
	 }
         if ((tab[i_tab][i_mot] = (char *) malloc (sizeof(char) * STEP)) == NULL) {
            tab[i_tab+1] = NULL;
            *flag = -1;
            free(line);
            return tab;
	 }
         i_car = 0;
         while (!isspace (line[i]) && !separator(line[i])){
            if (i_car == t_car -1) {
               t_car = t_car+STEP;
               tmp_car = (char *) realloc (tab[i_tab][i_mot],sizeof(char) * t_car);
               if (tmp_car == NULL) {
                  tab[i_tab][i_mot][i_car] = '\0';
                  tab[i_tab][i_mot+1] = NULL;
                  tab[i_tab+1] = NULL;
                  *flag = -1;
                  free(line);
                  return tab;
	       }
               tab[i_tab][i_mot] = tmp_car;
	    }
            tab[i_tab][i_mot][i_car] = line[i];
            i++;
            i_car++;
	 }
         tab[i_tab][i_mot][i_car] = '\0';
         while (isspace(line[i])) i++;
         i_mot++;
      }
      tab[i_tab][i_mot] = NULL;
      i_tab++;
      if (line [i] == '|') {
         i ++;
         while (isspace(line[i])) i++;
         if (separator(line[i])) {
            fprintf(stderr, "syntax error\n");
            free(line);
            return NULL;
	 }
      }
   }
   tab [i_tab] = NULL;
   if (line[i] == '\0') * flag = 0;
   else *flag = 1;
   free(line);
   *nb = i_tab;
   return tab;
}

