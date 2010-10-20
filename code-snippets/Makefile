BINARIES=openssl_test termio_test

all: ${BINARIES}

openssl_test: openssl_test.c
	$(CC) $< `pkg-config openssl --cflags --libs` -o $@

clean:
	rm -f ${BINARIES}
