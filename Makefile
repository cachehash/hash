.PHONY: all

LDLIBS+=-L. -lhash
CFLAGS+=-std=gnu99 -O3 -g


all: libhash.a

test.c: libhash.a
	
	
libhash.a: hash.o
	ar rcs $@ $^

clean:
	rm -f *.a *.o test
