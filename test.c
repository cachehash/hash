#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void showMap(Map *m) {
	for (Bucket* b = m->buckets; b; b=b->link) {
		printf("%s->", b->key);
	}
	printf("\n");
}
int main() {
	Map* m = newStrMap(1);
	int x = m->key.hashCode("hello");
	printf("%d\n", x);
	mPut(m,"hello", strdup("hi"));
	mPut(m,"bye", strdup("byzies"));
	mPut(m, "a", strdup("w"));
	mPut(m, "b", strdup("x"));
	mPut(m, "c", strdup("y"));
	mPut(m, "d", strdup("z"));
	printf("%s\n", (char*)mGet(m,"hello"));
	printf("%s\n", (char*)mGet(m,"bye"));
	showMap(m);
	mDel(m,"hello");
	showMap(m);
	mDel(m,"b");
	showMap(m);
	mDel(m,"d");
	showMap(m);
	mDel(m,"c");
	showMap(m);
	mDel(m,"a");
	showMap(m);
	mDel(m,"bye");
	showMap(m);
	printf("%ld\n", (long)mGet(m,"hello"));
}



