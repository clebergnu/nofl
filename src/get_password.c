/*
 * This file is part of nofl
 * Copyright (C) 2009 Cleber Rodrigues <cleber.gnu@gmail.com>
 * All rights reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 * 
 * Author(s): Cleber Rodrigues <cleber.gnu@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "get_password.h"

static struct termios *get_termio_attr(void) {
  struct termios *current;

  /* allocate space for termios structure */
  current = malloc(sizeof(struct termios));

  tcgetattr(STDIN_FILENO, current);

  return current;
}

static void enable_echo(void) {
  struct termios *current;

  current = get_termio_attr();
  current->c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, 0, current);
 
  free(current);
}

static void disable_echo(void) {
  struct termios *current;

  current = get_termio_attr();
  current->c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, 0, current);  

  free(current);
}

void strip_newline(char *string, int len) {
  if (string[len-1] == '\n')
    string[len-1] = '\0';
}

char *
get_password(const char *prompt) {
  char *input = NULL;
  size_t len = 0;
  ssize_t read;

  printf(prompt);
  disable_echo();
  read = getline(&input, &len, stdin);
  strip_newline(input, read);
  printf("\n");
  enable_echo();

  return input;
}

char *
get_password_if_match(const char *first_prompt, 
		      const char *second_prompt) {

  char *password1, *password2;
  
  password1 = get_password(first_prompt);
  password2 = get_password(second_prompt);

  if (strcmp(password1, password2) == 0) {
    free(password2);
    return password1;
  }

  free(password1);
  free(password2);
  return NULL;
}

char *
get_password_for_key(void) {

  char *password;

  password = get_password_if_match("Enter password to encrypt DSA key: ",
				   "Re-enter password (verification): ");

  if (!password) {
    printf("Passwords do NOT match\n");
    return NULL;
  } else {
    return password;
  }
}
