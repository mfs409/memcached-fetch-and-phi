#include <iostream>
#include <libmemcached/memcached.h>


using namespace std;

const char* conf_str = "--SERVER=6pac1:11211";

int main(int argc, char* argv[]) {

	if(argc < 2) {
		cout << "USAGE: " << argv[0] << " KEY" << endl;
		return -1;
	}

	memcached_st *memc = memcached(conf_str, strlen(conf_str));

	//checks for successful connection
	if(memc) {
		//some info for performing a get
		char* key = argv[1]; //key to look for

		//these are passed as pointers
		size_t val_size; //holds the length of the result
		uint32_t flags; //any keys you set in the cached
		memcached_return_t rc; //the result of 

		if(argc > 2 && strcmp(argv[2], "set") == 0)
		{
			rc = memcached_set(memc, key, strlen(key), "SUP", 3, 0, 0);	
			return 0;
		}
		//returns value as a char*, MUST BE FREED
		char* res = memcached_get(memc, key, strlen(key), &val_size, &flags, &rc);
		
		if(rc == MEMCACHED_NOTFOUND) {
			cout << "Key " << key << " not found in cache." << endl;
		}
		else if(rc != MEMCACHED_SUCCESS) {
			cout << "No go, try again..." << endl;
			cout << memcached_strerror(memc, rc) << endl;
		}
		else {
			size_t val_len;
			char* val = memcached_mod(memc, argv[1], strlen(argv[1]), "./strfcn.so", argv[2], &val_len, &rc, true);
			cout << "Worked? " << endl;
			free(res);
			if(val_len == 0) {
				cout << "Err: " << memcached_strerror(memc, rc) << endl;
			}
			else {
				cout << "Modded " << key << ": " << val << endl;
				//char* res = memcached_get(memc, key, strlen(key), &val_size, &flags, &rc);
				free(val);
			}
		}	
	
		memcached_free(memc);
	}	
	else {
		cout << "Connection failed." << endl;
	}

	return 0;

}
