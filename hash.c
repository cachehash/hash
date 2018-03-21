#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
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
int thinCmp(void* a, void*b) {
	return a == b;
}
void dummyFree(void* v) {
}
void* dummyDup(void* v) {
	return v;
}

int numHashCode(void* v) {
	size_t n = (size_t) v;
	int ret = 0;
	while (n) {
		ret ^= n;
		n >>= sizeof(int)*8;
	}
	return ret;
}

Map* newMap(int cap, DataHandlr key) {
	Map* m = calloc(sizeof(Map)+cap*sizeof(Bucket), 1);
	m->cap = cap;
	m->key = key;
	m->val.free = dummyFree;
	m->val.dup = dummyDup;
	return m;
}
Map* newIntMap(int cap) {
	DataHandlr dh;
	dh.hashCode = (HashFunc) numHashCode;
	dh.cmp = thinCmp;
	dh.dup = dummyDup;
	dh.free = dummyFree;
	return newMap(cap, dh);
}

Map* newStrMap(int cap) {
	DataHandlr dh;
	dh.hashCode = (HashFunc) stringHashCode;
	dh.cmp = stringCmp;
	dh.dup = (DupFunc) strdup;
	dh.free = free;
	return newMap(cap, dh);
}
Map* newStrRefMap(int cap) {
	DataHandlr dh;
	dh.hashCode = (HashFunc) stringHashCode;
	dh.cmp = stringCmp;
	dh.dup = dummyDup;
	dh.free = dummyFree;
	return newMap(cap, dh);
}

void unchainBucketRef(Map* m, Bucket* b) {
	b = b->listLink;
	if (!b) {
		printf("ERROR: b is null!\n");
	}
	Bucket* next = b->link;
	Bucket* prev = b->listLink;
	if (next) {
		next->listLink = b->listLink;
	}
	if (prev) {
		prev->link = b->link;
	}
	free(b);
}
void chainBucketRef(Map* m, Bucket* b, size_t needFreeOnDestroy) {
	Bucket* n = newLink();
	n->val = b;
	n->key = (void*)needFreeOnDestroy;
	b->listLink = n;
	n->link = m->link;
	if (m->link) {
		m->link->listLink = n;
	}
	m->link = n;
}
void mPut(Map* m, void *key, void* val){
	int i = m->key.hashCode(key) % m->cap;
	Bucket* b = &m->buckets[i];
	Bucket* bprev = NULL;
	val = m->val.dup(val);
	while (b != NULL) {
		if (b->key == NULL) {
			b->key = m->key.dup(key);
			b->val = val;
			chainBucketRef(m, b, 0);
			m->size++;
			return;
		} else if (m->key.cmp(b->key, key)) {
			m->val.free(b->val);
			b->val = val;
			return;
		}
		bprev = b;
		b = b->link;
	}
	b = bprev->link = newLink();
	chainBucketRef(m, b, 1);
	b->key = m->key.dup(key);
	b->val = val;
	m->size++;
}

void* mGet(Map* m, void *key) {
	int i = m->key.hashCode(key) % m->cap;
	Bucket* b = &m->buckets[i];
	while (b != NULL) {
		if (b->key != NULL && m->key.cmp(b->key, key)) {
			return b->val;
		}
		b = b->link;
	}
	return NULL;
}

void mDel(Map* m, void *key) {
	int i = m->key.hashCode(key) % m->cap;
	Bucket *buk = &m->buckets[i];
	Bucket *b = buk;
	Bucket* bprev = b;
	while (b != NULL) {
		if (b->key != NULL && m->key.cmp(b->key, key)) {
			void* ret = b->val;
			unchainBucketRef(m, b);
			m->key.free(b->key);
			bprev->link = b->link;
			if (b == buk) {
				if (b->link != NULL) {
					Bucket *blink = b->link;
					memcpy(b, b->link, sizeof(*b));
					//we moved where blink is in memory, so update reference
					b->listLink->val = b;
					b->listLink->key = (void*)0;
					free(blink);
				} else {
					memset(b, 0, sizeof(*b));
				}
			} else {
				free(b);
			}
			m->size--;
			m->val.free(ret);
			return;
		}
		bprev = b;
		b = b->link;
	}
}

void mGetKeys(Map* m, void **array) {
	int x = m->size;
	for (Bucket* b = m->link; b; b = b->link) {
		Bucket* buk = b->val;
		array[--x] = buk->key;
	}
}
void destroyMap(Map* m) {
	for (Bucket* b = m->link; b;) {
		Bucket* blink = b->link;;
		Bucket* buk = b->val;
		m->key.free(buk->key);
		m->val.free(buk->val);
		if (b->key) {
			free(buk);
		}
		free(b);
		b = blink;
	}
	free(m);
}
