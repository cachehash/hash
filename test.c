#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void showMap(Map *m) {
	for (int i = 0; i < m->cap; i++) {
		int printed = 0;
		for (Bucket* b = &m->buckets[i]; b; b=b->link) {
			if (b->key) {
				printed = 1;
				printf("%s->", b->key);
			}
		}
		if (printed) {
			printf("\n");
		}
	}
	printf("%d\n\n",m->size);
}
int main() {
	Map* m = newStrMap(2);
	int x = m->key.hashCode("hello");
	printf("%d\n", x);
	mPut(m,"hello", "hi");
	mPut(m,"bye", "byzies");
	mPut(m, "a", "w");
	mPut(m, "b", "x");
	mPut(m, "c", "y");
	mPut(m, "d", "z");
	mPut(m, "d", "dead");
	printf("%s\n", (char*)mGet(m,"hello"));
	printf("%s\n", (char*)mGet(m,"bye"));
	showMap(m);

	mDel(m,"hello");
	showMap(m);
	mDel(m,"b");
	showMap(m);

	void* vals[m->size];
	mGetKeys(m, vals);
	for (int x = 0; x < m->size; x++) {
		printf("%s is mapped to %s.\n", vals[x], mGet(m, vals[x]));
	}
	destroyMap(m);
	return 0;

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



