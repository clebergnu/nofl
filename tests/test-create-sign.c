#include <stdio.h>
#include <stdlib.h>

#include "init.h"
#include "certificate.h"
#include "sign.h"

int
main (int argc, char *argv[]) {

  int init_status;
  nofl_certificate *cert;

  init_status = nofl_initialize();

  /* create */
  cert = nofl_certificate_create("TEST_CERT");

  nofl_certificate_set_version(cert, "TEST_VERSION");
  nofl_certificate_set_customer_name(cert, "TEST_CUSTOMER_NAME");
  nofl_certificate_set_customer_id(cert, "TEST_CUSTOMER_ID");
  nofl_certificate_set_product_name(cert, "TEST_PRODUCT_NAME");
  nofl_certificate_set_product_version(cert, "TEST_PRODUCT_VERSION");
  nofl_certificate_set_product_min_version(cert, "TEST_PRODUCT_MIN_VERSION");
  nofl_certificate_set_product_max_version(cert, "TEST_PRODUCT_MAX_VERSION");
  nofl_certificate_set_expire_date(cert, "TEST_EXPIRE_DATE");
  nofl_certificate_set_expire_grace(cert, "TEST_EXPIRE_GRACE");

  /* set parms */
  cert->key_path = argv[1];

  /* sign */
  nofl_sign_certificate(cert);

  /* dump */
  nofl_certificate_dump(cert);  

  nofl_shutdown();
}
