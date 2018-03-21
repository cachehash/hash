#ifndef HASH_H
#define HASH_H

typedef struct Bucket {
	struct Bucket*		link;
	struct Bucket*		listLink;
	void*			key;
	void*			val;
} Bucket;

typedef int (*CmpFunc)(void*, void*);
typedef unsigned int (*HashFunc)(void*);
typedef void* (*DupFunc)(void*);
typedef void (*FreeFunc)(void*);
typedef struct DataHandlr {
	HashFunc	hashCode;
	CmpFunc		cmp;
	DupFunc		dup;
	FreeFunc	free;
} DataHandlr;
typedef struct Map {
	int		cap;
	int		size;
	Bucket*		link;
	DataHandlr	key;
	DataHandlr	val;
	Bucket		buckets[];
} Map;
Bucket* newLink();
Map* newMap(int cap, DataHandlr key);
Map* newStrMap(int cap);
Map* newIntMap(int cap);
Map* newStrRefMap(int cap);
void mPut(Map* m, void *key, void* val);
void* mGet(Map* m, void *key);
void mDel(Map* m, void *key);

void mGetKeys(Map* m, void **array);
void destroyMap(Map* m);


void dummyFree(void*);
void* dummyDup(void*);

#endif
