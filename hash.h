#ifndef HASH_H
#define HASH_H
int hashCode(char*);

typedef struct Bucket {
	struct Bucket*		link;
	char*			key;
	void*			val;
} Bucket;

typedef int (*CmpFunc)(void*, void*);
typedef int (*HashFunc)(void*);
typedef void* (*DupFunc)(void*);
typedef struct Map {
	int		size;
	HashFunc	hashCode;
	CmpFunc		keyCmp;
	DupFunc		keyDup;
	Bucket		buckets[0];
} Map;
Bucket* newLink();
Map* newMap(int size, HashFunc hashCode, CmpFunc keyCmp, DupFunc keyDup);
Map* newStrMap(int size);
void* mPut(Map* m, void *key, void* val);
void* mGet(Map* m, void *key);

#endif
