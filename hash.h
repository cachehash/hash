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
typedef void (*FreeFunc)(void*);
typedef struct DataHandlr {
	HashFunc	hashCode;
	CmpFunc		cmp;
	DupFunc		dup;
	FreeFunc	free;
} DataHandlr;
typedef struct Map {
	int		size;
	DataHandlr	key;
	Bucket		buckets[0];
} Map;
Bucket* newLink();
Map* newMap(int size, DataHandlr key);
Map* newStrMap(int size);
void* mPut(Map* m, void *key, void* val);
void* mGet(Map* m, void *key);
void* mDel(Map* m, void *key);

#endif
