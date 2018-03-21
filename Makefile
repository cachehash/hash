.PHONY: all

LDLIBS+=-L. -lhash
CFLAGS+=-std=gnu99 -O0 -g


all: libhash.a

test.c: libhash.a
	
	
libhash.a: hash.o
	ar rcs $@ $^

clean:
	rm -f *.a *.o test
