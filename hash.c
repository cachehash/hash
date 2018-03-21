#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int stringHashCode(char *str) {
	int h = 0;
	for (int i = 0; str[i]; i++) {
		h = 31*h+str[i];
	}
	return h;
}

Bucket* newLink() {
	return calloc(sizeof(Bucket), 1);
}

int stringCmp(void *a, void *b) {
	return strcmp(a,b) == 0;
}

Map* newMap(int size, HashFunc hashCode, CmpFunc keyCmp, DupFunc keyDup) {
	Map* m = calloc(sizeof(Map)+size*sizeof(Bucket), 1);
	m->hashCode = hashCode;
	m->keyCmp = keyCmp;
	m->keyDup = keyDup;
	m->size = size;
	return m;
}

Map* newStrMap(int size) {
	return newMap(size, (HashFunc)stringHashCode, stringCmp, (DupFunc) strdup);
}

void* mPut(Map* m, void *key, void* val){
	int i = m->hashCode(key) % m->size;
	Bucket* b = &m->buckets[i];
	Bucket* bprev = NULL;
	while (b != NULL) {
		if (b->key == NULL) {
			b->key = m->keyDup(key);
			b->val = val;
			return NULL;
		} else if (m->keyCmp(b->key, key)) {
			void* ret = b->val;
			b->val = val;
			return ret;
		}
		bprev = b;
		b = b->link;
	}
	b = bprev->link = newLink();
	b->key = m->keyDup(key);
	b->val = val;
	return NULL;
}

void* mGet(Map* m, void *key) {
	int i = m->hashCode(key) % m->size;
	Bucket* b = &m->buckets[i];
	while (b != NULL) {
		if (b->key != NULL && m->keyCmp(b->key, key)) {
			return b->val;
		}
		b = b->link;
	}
	return NULL;
}
