#include <stdio.h>
#include "fcn_map.h"

typedef struct Entry Entry;
typedef struct Bucket Bucket;

struct Entry {
	fcn_handle handle;
	char *key;
};

struct Bucket {
	unsigned int size;
	Entry *entries;
};

struct fcn_map {
	unsigned int size;
	Bucket *buckets;
};

//prototypes
static Entry* get_entry(Bucket *bucket, const char *key, int len);
static unsigned long djb2_hash(const char *str, int len);

//return a pointer to a new fcn_map
fcn_map* fcn_map_create(int num_buckets) {
	fcn_map *fm = (fcn_map*) malloc(sizeof(fcn_map));
	if(fm == NULL) return NULL;

	fm->size = num_buckets;
	fm->buckets = (Bucket*) malloc(sizeof(Bucket) * num_buckets);
	if(fm->buckets == NULL) {
		free(fm);
		return NULL;
	}

	memset(fm->buckets, 0, sizeof(Bucket) * num_buckets); //make sure buckets are zeroed
	return fm;
}

//free the fcn_map
void fcn_map_free(fcn_map *d) {
	Bucket *bucket = d->buckets;
	Entry *e;
	for(unsigned int i = 0, n = d->size; i < n; ++i) {
		e = bucket->entries;
		for(unsigned int j = 0, k = bucket->size; j < k; ++j) {
			free(e->key);
		}	
		free(bucket->entries);
		bucket++;
	}
	free(d->buckets);
	free(d);
}

fcn_handle fcn_map_get_n(fcn_map *fm, const char *key, int len) {
	unsigned int i;
	Bucket *bucket;
	Entry *entry;

	i = djb2_hash(key, len) % fm->size; //gets bucket index for key
	bucket = &(fm->buckets[i]);

	entry = get_entry(bucket, key, len);
	if(entry == NULL) return NULL; //not found

	return entry->handle; //give'em the handle
}

fcn_handle fcn_map_get(fcn_map *fm, const char *key) {
	return fcn_map_get_n(fm, key, strlen(key));	
}

//fcn_handle defined in fcn_map.h
unsigned int fcn_map_set_n(fcn_map *fm, const char *key, int len, fcn_handle handle) {
	unsigned int key_len = strlen(key);
	unsigned int i;
	char *cpy_key;
	Bucket *bucket;
	Entry *e;
	
	i = djb2_hash(key, len) % fm->size;
	
	bucket = &(fm->buckets[i]);
	e = get_entry(bucket, key, len);
	if(e != NULL) {
		//e is already in the map, update it's contents
		e->handle = handle;
		return 1;
	}
	else {

		//new entry, allocate a new one in the bucket
		cpy_key = (char*) malloc(key_len + 1); //add one for null
		if(cpy_key == NULL) return 0;

		if(bucket->size == 0) {
			//empty bucket, new allocate
			bucket->entries = (Entry*) malloc(sizeof(Entry));
			if(bucket->entries == NULL) {
				free(cpy_key);
				return 0;
			}
			bucket->size = 1;
		}
		else {
			//non-empty bucket, make more room
			Entry *t = (Entry*) realloc(bucket->entries, sizeof(Entry)*(bucket->size + 1));
			if(t == NULL) {
				free(cpy_key);
				return 0;
			}
			bucket->size++;
			bucket->entries = t;
		}

		//copy info into last pair in bucket
		e = &(bucket->entries[bucket->size - 1]);
		e->key = cpy_key;
		strncpy(e->key, key, key_len);	
		e->handle = handle;
		return 1;	
	}
}

unsigned int fcn_map_set(fcn_map *fm, const char *key, fcn_handle handle) {
	return fcn_map_set_n(fm, key, strlen(key), handle);
}

static Entry* get_entry(Bucket* bucket, const char *key, int len) {
	unsigned int size = bucket->size;
	if(size == 0) return NULL;
	
	Entry* e = bucket->entries;
	for(unsigned i = 0; i < size; ++i) {
		if(e->key != NULL && strncmp(e->key, key, len) == 0) {
			return e;
		}
		e++;
	}
	return NULL;
}

static unsigned long djb2_hash(const char *str, int len) {
	unsigned long hash = 5381;
	int c;
	while(len-- > 0) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}







