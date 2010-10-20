#include <stdio.h>
#include <stdlib.h>

#include "get_password.h"

int main(int argc, char *argv[]) {

  char *password;

  password = get_password_if_match("Enter password: ",
				   "Re-enter password: ");

  if (password) {
    printf("Passwords match! (\"%s\")\n", password);    
    free(password);
  }

}
