#include "certificate.h"

int
main(int argc, char *argv[]) {

  nofl_certificate *cert;

  /* Create cert */
  cert = nofl_certificate_create("Company A");

  /* Set misc attributes */
  nofl_certificate_set_version(cert, "TEST_VERSION");
  nofl_certificate_set_customer_name(cert, "TEST_CUSTOMER_NAME");
  nofl_certificate_set_customer_id(cert, "TEST_CUSTOMER_ID");
  nofl_certificate_set_product_name(cert, "TEST_PRODUCT_NAME");
  nofl_certificate_set_product_version(cert, "TEST_PRODUCT_VERSION");
  nofl_certificate_set_product_min_version(cert, "TEST_PRODUCT_MIN_VERSION");
  nofl_certificate_set_product_max_version(cert, "TEST_PRODUCT_MAX_VERSION");
  nofl_certificate_set_expire_date(cert, "TEST_EXPIRE_DATE");
  nofl_certificate_set_expire_grace(cert, "TEST_EXPIRE_GRACE");

  /* Dump the cert */
  nofl_certificate_dump(cert);
  nofl_certificate_destroy(cert);

  return 0;
}
