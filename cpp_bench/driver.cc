/*
 *This program runs benchmarking for our builds of memcached. 
 */

#include <sstream>
#include <fstream>
#include <iostream>
#include <thread>
#include <libmemcached/memcached.h>
#include <string.h>
#include <list>
#include <map>
#include <atomic>
#include <chrono>
#include <random>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

//local
#include "bmconfig.h"
#include "test.pb.h"
#include "timing.h"
#include "protostream.h"

const int MEGABYTE = 1024*1024;
const int typeBytes[] = {MEGABYTE/32, MEGABYTE/16, MEGABYTE/8, MEGABYTE/4, MEGABYTE/2, MEGABYTE};
const int KEY_LEN = 12;
const char* muteField = "hello world";
const char* muteFieldDiff = "HELLO WORLD";
std::map<std::string, int> keyMap;

void toupper_str(char* str, int n) {
	char diff = 'a' - 'A';
	for(char* ptr = str; ptr < str+n; ptr++) {
		if(*ptr > 'a' && *ptr < 'z') {
			*ptr -= diff;
		}
	}
}

void printLoad() {
	for(auto i = keyMap.begin(), e = keyMap.end(); i != e; i++) {
		std::cout << "Objects of type " << i->first << " (" << typeBytes[stoi(i->first)] << " bytes): " << i->second << std::endl;
	}
}

std::string getRandomKey(unsigned int* seed) {
	//grab random key using information in keyMap
	int typeI = rand_r(seed)%keyMap.size();
	std::string type;
	std::stringstream ss;
	ss.fill('0');
	ss.width(2);
	ss << typeI;
	ss >> type;

	int idRange = keyMap[type];
	int idI = rand_r(seed)%idRange;
	std::string id;
	ss.clear();
	ss.width(6);
	ss << idI;
	ss >> id;

	return type+id;
}

//Verifies that a given message is valid
bool valid(test::Entry& e) {
	int data = e.data();
	if(e.mute() != muteField) return false;
	if(data != e.arr_size()) return false;
	for(int i = 0, len = e.arr_size(); i < len; ++i) {
		if(data != e.arr(i)) return false;
	}
	return true;
}

bool validMutated(test::Entry& e) {
	int data = e.data();
	if(e.mute() != muteFieldDiff) return false;
	if(data != e.arr_size()) return false;
	for(int i = 0, len = e.arr_size(); i < len; ++i) {
		if(data != e.arr(i)) return false;
	}
	return true;
}

//Creates a load of given type (size specified in typeBytes[]) out of empty message e. Returns an the key to this message
std::string makeLoad(int type, test::Entry& e){
	//It takes approximately 5 bytes per entry in the message
	int num_entries = typeBytes[type]/5;
	e.set_mute(muteField);
	e.set_data(num_entries);
	for(int i = 0; i < num_entries; ++i) {
		e.add_arr(num_entries);
	}

	//create a string corresponding to this load	
	//keys are of the form DDNNNNNN, where DD is a two digit object type (determing later)
	//and NNNNNN is the id for an object of type DD
	std::stringstream ss;
	std::string key;
	ss.fill('0');
	ss.width(2);
	ss << std::right << type;
	ss >> key;

	int numDD = keyMap[key]++;
	std::string id;
	ss.clear(); //reset the stream
	ss.width(6);
	ss << std::right << numDD;
	ss >> id;
	return key+id;
	
}

void makeKey(char* key, int key_length, int threadNum, int objNum) {
	snprintf(key, key_length, "%02d-%09d", threadNum, objNum);
}

/* Initialize loads based on
 *	number of objects (num_objects)
 *	object distribution (size_dist)
 *	average size (size_objects)
 */
//NOTE: Restart memcached server between runs with different object sizes. Otherwise we start evicting our load
bool initLoads(Config& config) {
	memcached_st* memc = memcached(config.con_str.c_str(), config.con_str.length());
	//Memcached has 64MB of space by default. Packing efficiency can be as bad as 60% not counting key storage. 
	for(int k = 0; k < config.num_threads; ++k) { //init a new load for each combination of threads
		for(int i = 0; i < config.obj_per_thread; ++i) {
			char key[KEY_LEN+1]; //key is TT-NNNNNNNNN size 12 total
			makeKey(key, KEY_LEN+1, k, i);
			char val[config.size_objects];
			memset(val, 'd', config.size_objects);
			memcached_return_t rc = memcached_set(memc, key, KEY_LEN, val, config.size_objects, 0, 0); //don't expire, default flags
			if(rc != MEMCACHED_SUCCESS) {
				std::cout << "Failed to insert key " << key << ": " << memcached_strerror(memc, rc) << std::endl;
				memcached_free(memc);
				return false;
			}	
		}
	}
	memcached_free(memc);
	return true;
}

void initLoadsProto(Config& config) {
	memcached_st* memc = memcached(config.con_str.c_str(), config.con_str.length());
	//for now just make n objects of given size
	for(int k = 0; k < 3; ++k) {
		for(int i = 0; i < config.num_objects/3; ++i) {
			test::Entry e;
			std::string key = makeLoad(k, e);
			std::string val;
			e.SerializeToString(&val);
			
			memcached_return_t rc = memcached_set(memc, key.c_str(), key.length(), val.c_str(), val.length(), 0, 0);
			if(rc != MEMCACHED_SUCCESS) {
				std::cout << "Failed to insert the " << i << "th element." << std::endl;
			}	
		}
	}

	memcached_free(memc);
}

//the input line to be parsed, and the resulting argv which may be used for executing commands
void parseArgs(char* input, char** argv) {
	while(*input != '\0') {
		//treat all leading whitespace
		while(*input == ' ' || *input == '\r' || *input == '\n' || *input == '\t') {
			*input = '\0';
			input++;
		}
		//non-whitespace found, set start of next argument
		*argv = input;
		argv++;
		//skip util next non-whitespace or null
		while(*input != ' ' && *input != '\r' && *input != '\n' && *input != '\0') {
			input++;
		}
	}
	*argv = '\0'; //marks end of list
}

//create a parallel process using argv. Returns the PID of the process to be killed by the parent. 
pid_t execProg(char** argv) {
	pid_t pid = 0;
	if((pid = fork()) < 0) {
		std::cerr << "Error forking child.\n";
	}
	else if(pid == 0) {
		//child process, switch exec
		execvp(*argv, argv); //TODO: should quit child on failure
	}
	else {
		return pid;
	}
}

void mod_bench(unsigned int thread_num, std::string con_str, int num_objects, bool reply) {
        memcached_st* memc = memcached(con_str.c_str(), con_str.length());

        if(!memc) {
                std::cout << "Could not connect using " << con_str << std::endl;
                return;
        }

	char key[KEY_LEN+1]; //array for holding keys
        //TODO: Catch failed operations and record for stats
        for(int i = 0; i < num_objects; ++i) {
                //get an object, parse it, validate it, serialize it, set it.
        	makeKey(key, KEY_LEN+1, thread_num, i); //make key for this thread and object        
		size_t vSize;
                uint32_t flags;
                time_t exp = 0; //leaving this uninitialized seemed to screw things up.
                memcached_return_t rc;

                char* val = memcached_mod(memc, key, KEY_LEN, "./strfcn.so", "toupper_str", &vSize, &rc, reply);

                if(rc != MEMCACHED_SUCCESS) {
			std::cout << "Mod failed on key " << key << " ";
			std::cout << "Err: " << memcached_strerror(memc, rc) << std::endl;	
        		if(val) free(val);
			return;
	        }
		
		if(val) free(val);
        }
        memcached_free(memc);
}


//function for performing a set number of get/set pairs
void default_bench(unsigned int thread_num, std::string con_str, int num_objects) {
        memcached_st* memc = memcached(con_str.c_str(), con_str.length());

        if(!memc) {
                std::cout << "Could not connect using " << con_str << std::endl;
                return;
        }

	char key[KEY_LEN+1]; //array for holding keys
        //TODO: Catch failed operations and record for stats
        for(int i = 0; i < num_objects; ++i) {
                //get an object, parse it, validate it, serialize it, set it.
        	makeKey(key, KEY_LEN+1, thread_num, i); //make key for this thread and object        
		size_t vSize;
                uint32_t flags;
                time_t exp = 0; //leaving this uninitialized seemed to screw things up.
                memcached_return_t rc;

                char* v = memcached_get(memc, key, KEY_LEN, &vSize, &flags, &rc);

                if(rc == MEMCACHED_SUCCESS) {
			toupper_str(v, vSize); //modify the string
			rc = memcached_set(memc, key, KEY_LEN, v, vSize, flags, exp);
                        if(rc != MEMCACHED_SUCCESS) {
				std::cout << "Set failed on key " << key << std::endl;
				std::cout << "Set error: " << memcached_strerror(memc, rc) << std::endl;
                        }
                }
                else {
			//track failures here?
                        std::cout << "Invalid message retrieved. Key: " << key << std::endl;
			if(rc != MEMCACHED_SUCCESS) {
                                std::cout << "Get failed: " << memcached_strerror(memc, rc) << std::endl;
                        }
			if(v) free(v);
			return;
                }

                if(v) free(v);
        }
        memcached_free(memc);
}

void benchProto(unsigned int thread_num, std::string con_str, int num_ops, float ratio_ops) {
	memcached_st* memc = memcached(con_str.c_str(), con_str.length());
	
	if(!memc) {
		std::cout << "Could not connect using " << con_str << std::endl;
		return;
	}

	//TODO: Catch failed operations and record for stats
	for(int i = 0; i < num_ops; ++i) {
		//get an object, parse it, validate it, serialize it, set it.
		std::string key = getRandomKey(&thread_num);
		size_t vSize;
		uint32_t flags;
		time_t exp = 0; //leaving this uninitialized seemed to screw things up. 
		memcached_return_t rc;
		test::Entry e;
		
		char* v = memcached_get(memc, key.c_str(), key.length(), &vSize, &flags, &rc);
		protostream pin(v, v+vSize);
		
		if((rc == MEMCACHED_SUCCESS) && e.ParseFromIstream(&pin) && valid(e)) {
			std::string out;
			e.set_mute(muteFieldDiff);	
			if(e.SerializeToString(&out)) {
				rc = memcached_set(memc, key.c_str(), key.length(), out.c_str(), out.length(), flags, exp);
				if(rc != MEMCACHED_SUCCESS) {
					std::cout << "Set failed: " << memcached_strerror(memc, rc) << std::endl;
				}
			}
			else {
				std::cout << "Failed to serialize! Key: " << key << std::endl;
			}
		
		}
		else {
			std::cout << "Invalid message retrieved. Key: " << key << std::endl;
			if(rc != MEMCACHED_SUCCESS) {
				std::cout << "Get failed: " << memcached_strerror(memc, rc) << std::endl;
			}
		}

		if(v) free(v);
	}
	memcached_free(memc);
}

int main(int argc, char* argv[]) {
	
	GOOGLE_PROTOBUF_VERIFY_VERSION; //check for protobuf compatibility

	Config config;	
	config.parseargs(argc, argv, "Memcached Driver");
	if(config.disp_help) return 0;	
	config.obj_per_thread = (MEGABYTE*64*0.6)/config.size_objects/config.num_threads;

	pid_t pid;
	if(config.run_server) {
		char* argvp[64];
		char* line;
		
		config.makeExecLine(&line, 1024);
		//parses command line into char** for execvp
		parseArgs(line, argvp);
		//execute and store pid to remove when program is finished
		pid = execProg(argvp);
		free(line);
		
		//short pause to make sure memcached has time to get up and running
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	
	//INIT LOADS
	if(!initLoads(config)) return 0;	

	//create multiple threads and connect
	std::list<std::thread*> thread_list;
	//std::cout << "Conn string is " << config.con_str << std::endl;
	uint64_t start = getElapsedTime();
	//std::cout << "Starting default\n";
	for(unsigned int i = 0; i < config.num_threads; ++i) {
		thread_list.push_front(new std::thread(default_bench, i, config.con_str, config.obj_per_thread));
	}
	
	//wait for each thread to finish and clean up
	while(thread_list.size() > 0) {	
		std::thread* t = thread_list.front();
		t->join();
		thread_list.pop_front();
		free(t);
	}

	uint64_t default_bench_time = getElapsedTime() - start;
	//std::cout << "Total time (default): " << default_bench_time/1000000000.0 << std::endl;

	//RE-INIT LOADS
	if(!initLoads(config)) return 0;
	
	start = getElapsedTime();
	//std::cout << "Starting mod\n";
	for(unsigned int i = 0; i < config.num_threads; ++i) {
		thread_list.push_front(new std::thread(mod_bench, i, config.con_str, config.obj_per_thread, true));
	}
	
	//wait for each thread to finish and clean up
	while(thread_list.size() > 0) {	
		std::thread* t = thread_list.front();
		t->join();
		thread_list.pop_front();
		free(t);
	}

	uint64_t mod_bench_time_reply = getElapsedTime() - start;
	//std::cout << "Total time (mod): " << mod_bench_time_reply/1000000000.0 << std::endl;

	//RE-INIT LOADS
	if(!initLoads(config)) return 0;
	
	start = getElapsedTime();
	//std::cout << "Starting mod no reply\n";
	for(unsigned int i = 0; i < config.num_threads; ++i) {
		thread_list.push_front(new std::thread(mod_bench, i, config.con_str, config.obj_per_thread, false));
	}
	
	//wait for each thread to finish and clean up
	while(thread_list.size() > 0) {	
		std::thread* t = thread_list.front();
		t->join();
		thread_list.pop_front();
		free(t);
	}

	config.time = getElapsedTime() - start;
	//std::cout << "Total time (mod): " << config.time/1000000000.0 << std::endl;


	//print out csv: local/remote,client_thread_count,object_size,object_count,default_time,mod_time,mod_no_reply_time
	printf("%s,%d,%d,%u,%lu,%lu,%lu\n", (config.server == "localhost")?"local":"remote", config.num_threads,config.size_objects,config.obj_per_thread,default_bench_time,mod_bench_time_reply,config.time);

	if(config.run_server) {
		std::cout << "Memcached Server still running at " << config.server << ":" << config.port << std::endl;
		std::cout << "Press enter to exit\n";
		
		memcached_st* memc = memcached(config.con_str.c_str(), config.con_str.length());
		std::string dump;
		std::getline(std::cin, dump);
		while(dump.length() > 0) {
			//perform get
			char* val;
			size_t vSize;
			uint32_t flags;
			memcached_return_t rc;

			val = memcached_get(memc, dump.c_str(), dump.length(), &vSize, &flags, &rc);

			if(rc == MEMCACHED_SUCCESS) {
				protostream pin(val, val+vSize);
				test::Entry e;
				e.ParseFromIstream(&pin);
				if(valid(e)) std::cout << "Retrieved a valid message!\n";
				else if(validMutated(e)) std::cout << "Retrieved a valid mutated message!\n";
				else std::cout << "Retrieved an invalid message...\n";
			}
			else {
				std::cout << "Failed to get [" << dump << "]: " << memcached_strerror(memc, rc) << std::endl;
			}
			std::getline(std::cin, dump);
		}
		memcached_free(memc);
		kill(pid, SIGKILL); //kill memcached server
	}	

	//cleanup
	google::protobuf::ShutdownProtobufLibrary(); //protobuf cleanup
	return 0;	
}
