//a map for cstrings 

#ifndef _FCNMAP_H_
#define _FCNMAP_H_

#ifdef __cplusplus
extern "C" {//avoid mangling
#endif

#include <string.h>
#include <stdlib.h>

typedef struct fcn_map fcn_map;
typedef int (*fcn_handle)(char*, int, int, char*, int);

//allocate a new cmap 
fcn_map* fcn_map_create(int num_buckets);
//deallocate all memory in d
void fcn_map_free(fcn_map *d);

//returns the handle to the function as a void* (must convert before use)
fcn_handle fcn_map_get(fcn_map *m, const char *key);
fcn_handle fcn_map_get_n(fcn_map *m, const char *key, int len);

//adds the fcn_handle into the map. Overwrites existing handle if one already exists
unsigned int fcn_map_set(fcn_map *m, const char *key, fcn_handle handle);
unsigned int fcn_map_set_n(fcn_map *m, const char *key, int len, fcn_handle handle);

#ifdef __cplusplus
}
#endif
#endif //header guard
