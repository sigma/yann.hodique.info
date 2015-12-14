#include <stdio.h> 
#include "command_line.h"

int main (void) {
   int f;
   int nb;
   char ***l;
   while (1) {
      if ((l = commandLine(&f,&nb)) != NULL) {
         displayLine (l);
         printf ("-------------flag = %d-------------\n", f);
         freeLine(l);
      }
   }
   return 0;

}
