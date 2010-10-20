#include <stdio.h>
#include <stdlib.h>

#include "init.h"

int
main (int argc, char *argv[]) {

  int init_status;

  init_status = nofl_initialize();
  nofl_sign_certificate(argv[1], argv[2], argv[3]);
  nofl_shutdown();
}
