#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define MAX_INPUT 124

int
main(int argc, char *argv[]) {
  
  struct termios *current;
  int c_lflag_bkp;
  char line[MAX_INPUT];

  /* allocate space for termios structure */
  current = malloc(sizeof(struct termios));

  tcgetattr(STDIN_FILENO, current);

  printf("c_flags is: %i\n", (int) current->c_lflag);
  /* test echo */
  if (current->c_lflag && ECHO) {
    printf("ECHO is enabled\n");
  }

  c_lflag_bkp = current->c_lflag;
  current->c_lflag &= ~ECHO;
  printf("FLAGS is now: %i\n", (int) current->c_lflag);
  if (current->c_lflag && ~ECHO) {
    printf("ECHO is now disabled\n");
  }

  tcsetattr(STDIN_FILENO, 0, current);
  
  printf("Enter Password: ");
  fgets(line, MAX_INPUT, stdin);
  printf("\n");

  printf("Restoring saved settings\n");
  current->c_lflag = c_lflag_bkp;
  tcsetattr(STDIN_FILENO, 0, current);

  free(current);

  exit(0);
}
