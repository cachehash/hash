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
void dummyFree(void* v) {
}
void* dummyDup(void* v) {
	return v;
}

Map* newMap(int size, DataHandlr key) {
	Map* m = calloc(sizeof(Map)+size*sizeof(Bucket), 1);
	m->key = key;
	m->size = size;
	return m;
}

Map* newStrMap(int size) {
	DataHandlr dh;
	dh.hashCode = (HashFunc) stringHashCode;
	dh.cmp = stringCmp;
	dh.dup = (DupFunc) strdup;
	dh.free = free;
	return newMap(size, dh);
}
Map* newStrRefMap(int size) {
	DataHandlr dh;
	dh.hashCode = (HashFunc) stringHashCode;
	dh.cmp = stringCmp;
	dh.dup = dummyDup;
	dh.free = dummyFree;
	return newMap(size, dh);
}

void* mPut(Map* m, void *key, void* val){
	int i = m->key.hashCode(key) % m->size;
	Bucket* b = &m->buckets[i];
	Bucket* bprev = NULL;
	while (b != NULL) {
		if (b->key == NULL) {
			b->key = m->key.dup(key);
			b->val = val;
			return NULL;
		} else if (m->key.cmp(b->key, key)) {
			void* ret = b->val;
			b->val = val;
			return ret;
		}
		bprev = b;
		b = b->link;
	}
	b = bprev->link = newLink();
	b->key = m->key.dup(key);
	b->val = val;
	return NULL;
}

void* mGet(Map* m, void *key) {
	int i = m->key.hashCode(key) % m->size;
	Bucket* b = &m->buckets[i];
	while (b != NULL) {
		if (b->key != NULL && m->key.cmp(b->key, key)) {
			return b->val;
		}
		b = b->link;
	}
	return NULL;
}

void* mDel(Map* m, void *key) {
	int i = m->key.hashCode(key) % m->size;
	Bucket *buk = &m->buckets[i];
	Bucket *b = buk;
	Bucket* bprev = b;
	while (b != NULL) {
		if (b->key != NULL && m->key.cmp(b->key, key)) {
			void* ret = b->val;
			m->key.free(b->key);
			bprev->link = b->link;
			if (b == buk) {
				if (b->link != NULL) {
					Bucket *blink = b->link;
					memcpy(b, b->link, sizeof(*b));
					free(blink);
				} else {
					memset(b, 0, sizeof(*b));
				}
			} else {
				free(b);
			}
			return ret;
		}
		bprev = b;
		b = b->link;
	}
	return NULL;
}
