lib_LTLIBRARIES = libnofl.la libnoflverify.la
libnofl_la_SOURCES = init.c sign.c certificate.c keygen.c get_password.c verify.c
libnoflverify_la_SOURCES = init.c certificate.c verify.c

bin_PROGRAMS = nofl-manager 
nofl_manager_LDADD = libnofl.la

# testdir = /something
# test_PROGRAMS = test-sign test-certificate test-create-sign test-keygen
# test_verify_SOURCES = test_verify.c
# test_verify_LDADD = libnoflverify.la
# test_sign_LDADD = libnofl.la
# test_certificate_LDADD = libnofl.la
# test_create_sign_LDADD = libnofl.la
# test_keygen_LDADD = libnofl.la

