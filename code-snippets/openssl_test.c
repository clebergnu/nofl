#include <stdio.h>
#include <stdlib.h>

#include <openssl/evp.h>

int main(int argc, char *argv[]) {

  OpenSSL_add_all_algorithms();

  exit(0);
}
