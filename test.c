#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	Map* m = newStrMap(1);
	int x = m->hashCode("hello");
	printf("%d\n", x);
	mPut(m,"hello", strdup("hi"));
	mPut(m,"bye", strdup("byzies"));
	printf("%s\n", (char*)mGet(m,"hello"));
	printf("%s\n", (char*)mGet(m,"bye"));
}

