.PHONY: all

LDLIBS=-L. -lhash
CFLAGS=-Og -g

all: main

main.o:	test.o libhash.a
	cp test.o main.o
	
libhash.a: hash.o
	ar rcs $@ $^

clean:
	rm -f *.a *.o main
