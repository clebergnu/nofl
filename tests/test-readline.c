#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

int
main (int argc, char *argv[]) {

  char *line;

  int exit_flag = 0;

  do { 
    line = readline("Prompt> ");

    if (!strcmp(line, "exit"))
      exit_flag = 1;

  } while (exit_flag==0);


}
