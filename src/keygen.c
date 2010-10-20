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

#include <string.h>

#include "keygen.h"
#include "get_password.h"

void generate_rsa_keypair(char *password, char *privkey_path, char *pubkey_path) {
  FILE *pub;
  FILE *priv;
  RSA *rsa;

  rsa = RSA_generate_key(1024,	/* bits */
			 RSA_3,	/* e? */
			 NULL,	/* callback */
			 NULL);	/* callback arg */

  pub = fopen(pubkey_path, "w");
  priv = fopen(privkey_path, "w");

  PEM_write_RSA_PUBKEY(pub, 	   /* file pointer */
		       rsa, 	   /* dsa object */
		       NULL, 	   /* encryption type */
		       NULL,       /* encryption key string */
		       NULL,       /* encryption key string length */
		       NULL);	   /* ??? */

  if (password) {
    PEM_write_RSAPrivateKey(priv, rsa, EVP_aes_256_cbc(), password, strlen(password), NULL, NULL);
  } else {
    PEM_write_RSAPrivateKey(priv, rsa, NULL, NULL, NULL, NULL);
  }

  fclose(pub);
  fclose(priv);
}

void
generate_dsa_keypair(char *password, char *privkey_path, char *pubkey_path) {

  FILE *pub;
  FILE *priv;
  DSA *dsa;

  dsa = DSA_generate_parameters(1024,  /* bits */
				NULL,  /* seed */
				0,     /* seed len */
				NULL,  /* counter_ret */
				NULL,  /* h_ret */
				NULL,  /* callback */
				NULL); /* callback arg */

  DSA_generate_key(dsa);

  pub = fopen(pubkey_path, "w");
  priv = fopen(privkey_path, "w");

  PEM_write_DSA_PUBKEY(pub, 	   /* file pointer */
		       dsa, 	   /* dsa object */
		       NULL, 	   /* encryption type */
		       NULL,       /* encryption key string */
		       NULL,       /* encryption key string length */
		       NULL);	   /* ??? */

  if (password) {
    PEM_write_DSAPrivateKey(priv, dsa, EVP_aes_256_cbc(), password, strlen(password), NULL, NULL);
  } else {
    PEM_write_DSAPrivateKey(priv, dsa, NULL, NULL, NULL, NULL);
  }

  fclose(pub);
  fclose(priv);
}

void
generate_dsa_keypair_interactive(void) {

  char *password = NULL;
  char *pubkey_path = NULL;
  char *privkey_path = NULL;

  size_t len = 0;
  ssize_t read;

  password = get_password_for_key();

  if (!password) {
    return;
  }

  printf("Enter path to save the private key: ");
  read = getline(&privkey_path, &len, stdin);
  strip_newline(privkey_path, read);


  printf("Enter path to save the public key: ");
  read = getline(&pubkey_path, &len, stdin);
  strip_newline(pubkey_path, read);

  if (password && pubkey_path && privkey_path) {
    generate_dsa_keypair(password, privkey_path, pubkey_path);
  }
}
