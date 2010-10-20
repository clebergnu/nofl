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
#include <getopt.h>
#include <unistd.h>

#include "init.h"
#include "certificate.h"
#include "get_password.h"
#include "keygen.h"
#include "verify.h"

#undef DEBUG

/* 
 * Function definitions
 */
static int print_help(char *argv[]);
static int action_new_cert(void);
static int action_new_key(void);
static int action_sign(void);
static int action_verify(void);

/* 
 * The actions that have been provided in the command line
 */
#define ACTION_OPTION_HELP        1 << 0
#define ACTION_OPTION_NEW_CERT    1 << 1
#define ACTION_OPTION_NEW_KEY     1 << 2
#define ACTION_OPTION_SIGN        1 << 3
#define ACTION_OPTION_VERIFY      1 << 4
static int action_option = 0;

/* 
 * The certificate name (usually the name of the product) 
 */
static char *cert_product_name = NULL;
static char *cert_product_version = NULL;
static char *cert_customer_name = NULL;
static char *private_key = NULL;
static char *public_key = NULL;
static char *certificate = NULL;

/* 
 * If a certificate is being created, save it globally
 */
static nofl_certificate *cert = NULL;

/*
 * Init and Exit status
 */
static int init_status = 0;
static int exit_status = 0;

static int 
action_new_cert(void) 
{
  if (cert_product_name) {
    cert = nofl_certificate_create(cert_product_name);
    nofl_certificate_set_product_name(cert, cert_product_name);
  }
  if (cert_product_version) {
    nofl_certificate_set_product_version(cert, cert_product_version);
    nofl_certificate_set_product_min_version(cert, cert_product_version);
    nofl_certificate_set_product_max_version(cert, cert_product_version);
  }
  if (cert_customer_name)
    nofl_certificate_set_customer_name(cert, cert_customer_name);

  nofl_certificate_set_expire_grace(cert, 90);

  if (certificate == NULL)
    certificate = "/tmp/nofl-cert.crt";

  cert->output_path = certificate;
  nofl_certificate_save(cert);

  return 0;
}


static int 
action_new_key(void)
{
  char *password;

  password = get_password_for_key();
  if (password) {
    generate_rsa_keypair(password, private_key, public_key);
    return 0;
  }
  return -1;
}

static int 
action_sign(void)
{
  /* 
   * Load certificate
   */
  if (cert == NULL) {
    cert = nofl_certificate_load(certificate);
  }

  if (private_key)
    cert->key_path = private_key;

  if (certificate)
    cert->output_path = certificate;

  if (cert->key_path && cert->output_path) {
    nofl_sign_certificate(cert);
    nofl_certificate_save(cert);
    return 0;
  } else {
    return 1;
  }
}

static int 
action_verify(void) 
{
  int verify_result = -1;

  if (cert == NULL) {
    cert = nofl_certificate_load(certificate);
  }
  
  if (public_key) {
    verify_result = nofl_verify_certificate(cert, public_key);
    if (verify_result == 0)
      printf("Certificate Signature OK\n");
    else
      printf("Certificate Signaure FAILED\n");
  }
  
  return verify_result;
}

static void 
parse_args(int argc, char *argv[]) {

  int c; 
  int option_index = 0;

  static struct option long_options[] = {

    /* actions: some can be used togheter, some others cant */
    {"new-cert", no_argument, NULL, 'n'},
    {"new-key", no_argument, NULL, 'k'},
    {"sign", no_argument, NULL, 's'},
    {"verify", no_argument, NULL, 'v'},

    /* parameters: depending on the action some of these should be specified */
    {"certificate", required_argument, NULL, 0},
    {"product-name", required_argument, NULL, 0},
    {"product-version", required_argument, NULL, 0},
    {"customer-name", required_argument, NULL, 0},

    {"private-key", required_argument, NULL, 0},
    {"public-key", optional_argument, NULL, 0}, /* derive from private-key */

    {"help", no_argument, NULL, 'h'},

    {0, 0, 0, 0}
  };

  while(1) {
    c = getopt_long(argc, argv, "nksvh", long_options, &option_index);

    switch(c) {

    case 'n':
      action_option |= ACTION_OPTION_NEW_CERT;
      break;

    case 'k':
      action_option |= ACTION_OPTION_NEW_KEY;
      break;

    case 's':
      action_option |= ACTION_OPTION_SIGN;
      break;

    case 'v':
      action_option |= ACTION_OPTION_VERIFY;
      break;

    case 'h':
      action_option |= ACTION_OPTION_HELP;
      break;

    case 0:
      if(!strcmp(long_options[option_index].name, "product-name"))
	cert_product_name = optarg;
      else if(!strcmp(long_options[option_index].name, "product-version"))
	cert_product_version = optarg;
      else if(!strcmp(long_options[option_index].name, "customer-name"))
	cert_customer_name = optarg;
      else if(!strcmp(long_options[option_index].name, "private-key"))
	private_key = optarg;
      else if(!strcmp(long_options[option_index].name, "public-key"))
	public_key = optarg;
      else if(!strcmp(long_options[option_index].name, "certificate"))
	certificate = optarg;
	   
      break;
    }

    if (c == -1)
      break;
  }
}

static int print_help(char *argv[]) {

  char *argv_0, *cmd_name;

  argv_0 = argv[0];
  cmd_name = argv_0;

  /* 
   * strip ./ from cmd name
   */
  if ((argv_0[0] == '.') && (argv_0[1] == '/')) {
    cmd_name++; cmd_name++;
  }

  printf("This is NOFL (there's NO Free Lunch):\n");
  printf("A Free and Open Source Software Subscription Management System\n\n");
  printf("Usage: %s [actions] [options]\n\n", cmd_name);
  printf("Actions:\n");
  printf("  -k, --new-key       Creates new key pair (private and public)\n");
  printf("  -n, --new-cert      Creates new certificate\n");
  printf("  -s, --sign          Signs a certificate\n");
  printf("  -v, --verify        Verifys the signature on a certificate\n");
  printf("  -h, --help          Prints this help message\n");
  printf("\n");
  printf("Options (File Input/Ouput):\n");
  printf("  --cert-output       Save certificate at this path\n");
  printf("  --private-key       Save/load private key located at this path\n");
  printf("  --publib-key        Save/load public key located at this path\n");
  printf("\n");
  printf("Options (Certificate Info):\n");
  printf("  --product-name      Sets product name in certificate\n");
  printf("  --product-version   Sets product version in certificate\n");
  printf("  --customer-name     Sets customer name in certificate\n");
  printf("  --initial-date      Sets the initial date of the certificate\n");
  printf("  --expire-date       Sets the final valid date of the certificate\n");
  printf("\n");

  return 0;
}

static void check_options(void) {

  int options_ok = 0;

  if (action_option & ACTION_OPTION_NEW_CERT) {
    if (cert_product_name == NULL) {
      options_ok--;
      printf("ERROR: \"--product-name\" is mandatory when creating certificates\n");
    }
    if (cert_product_version == NULL) {
      options_ok--;
      printf("ERROR: \"--product-version\" is mandatory when creating certificates\n");
    }
    if (cert_customer_name == NULL) {
      options_ok--;
      printf("ERROR: \"--customer-name\" is mandatory when creating certificates\n");
    }
  }

  if ((action_option & ACTION_OPTION_NEW_KEY) || \
      (action_option & ACTION_OPTION_SIGN)) {
    if (private_key == NULL) {
      options_ok--;
      printf("ERROR: \"--private-key\" is mandatory when signing certificates\n");
    }
  }

  if (action_option & ACTION_OPTION_NEW_KEY) {
    if (public_key == NULL) {
      options_ok--;
      printf("ERROR: \"--public-key\" is mandatory when signing certificates\n");
    }
  }

  if (action_option & ACTION_OPTION_SIGN) {
    if (certificate == NULL) {
      options_ok--;
      printf("ERROR: \"--certificate\" is mandatory when signing certificates\n");
    }
  }

  if (action_option & ACTION_OPTION_VERIFY) {
    if (certificate == NULL) {
      options_ok--;
      printf("ERROR: \"--certificate\" is mandatory when signing certificates\n");
    }
    if (public_key == NULL) {
      options_ok--;
      printf("ERROR: \"--public-key\" is mandatory when signing certificates\n");
    }
  }

  if (options_ok < 0) 
    exit(options_ok);
}

int main(int argc, char *argv[]) {

  parse_args(argc, argv);

  /* 
   * Any valid action has been provided
   */
  if (action_option) {
    check_options();

    init_status = nofl_initialize();

    if (action_option & ACTION_OPTION_NEW_KEY)
      exit_status = action_new_key();
    if (action_option & ACTION_OPTION_NEW_CERT)
      exit_status = action_new_cert();
    if (action_option & ACTION_OPTION_SIGN)
      exit_status = action_sign();
    if (action_option & ACTION_OPTION_VERIFY)
      exit_status = action_verify();

    nofl_shutdown();
    return exit_status;

    if (action_option & ACTION_OPTION_HELP)
      return print_help(argv);
  }

  /* 
   * No valid action has been provided, present help
   */
  else {
    return print_help(argv);
  }
}
