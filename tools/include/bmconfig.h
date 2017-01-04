#pragma once

#include <iostream>
#include <atomic>
#include <string>
#include <unistd.h>

/**
 * Standard benchmark configuration globals
 */
#define MAX_CLIENT_THREADS 12

struct Config
{
	std::string	buildPath;	//the build path for the binary of memcached to execute
	std::string	port;		//the port to listen for memcached
	std::string	server;		//the server to look for memcached
	std::string	con_str;	//the string to be used for creating new connections with memcached servers
	bool		run_server;	//run a local server
	bool		run_server_binary;
	bool		disp_help;
	uint32_t	num_threads;
	uint32_t	num_ops;	//number of operations to run on each thread
	uint32_t	num_objects;	//number of objects to be stored
	uint32_t	size_objects;	//the size of an average object to be stored in bytes
   	uint32_t	obj_per_thread;	//number of objects each thread has
	float		size_dist;	//the size distribution according to a normal distribution
	float		ratio_ops;	//the ratio between get and set operations
	 
	//For final stats
	uint64_t	time;
   	std::atomic<bool>	running; 
	
	// Constructor just sets reasonable defaults for everything
	Config() :
		buildPath("memcached_build/bin/memcached"), port("11111"),
		server("localhost"), run_server(false), run_server_binary(false),
		num_threads(1), num_ops(1000),
		num_objects(100), size_objects(100000), obj_per_thread(100),
		size_dist(0.5), ratio_ops(1),
		disp_help(false), con_str("")
	{ }


	/// Print usage
	void usage(std::string name) {
		std::cerr << "Usage: " << name << "\n";
		std::cerr << "    -b: build path to memcached binary\n";
		std::cerr << "    -r: include to run local server\n";
		std::cerr << "    -B: run server with binary protocol\n";
		std::cerr << "    -s: server name to connect to\n";
		std::cerr << "    -p: port number to connect to\n";
		std::cerr << "    -S: server name and port pair (directly adds argument to connection string ex --SERVER=[localhost:11211]\n";
		std::cerr << "    -t: number of threads to use\n";
		std::cerr << "    -n: number of operations to run on each thread\n";
		std::cerr << "    -o: number of objects to initialize in cache\n";
		std::cerr << "    -k: the average size of an object\n";
		std::cerr << "    -d: the deviation of object sizes (normal distribution)\n";
		std::cerr << "    -i: ratio between get and set operations (0.5 indicates 1 get to every 2 sets)\n";
	}

	//TODO: Figure out how to easily configure connections to multiple servers at one time
	/// Parse command line arguments
	void parseargs(int argc, char** argv, std::string name) {
		int opt;
		while ((opt = getopt(argc, argv, "b:rBs:p:S:t:n:o:k:d:i:h")) != -1) {
		    switch(opt) {
			case 'b': buildPath	= strtol(optarg, NULL, 10); break;
			case 'r': run_server	= true; break;
			case 'B': run_server_binary = true; break;
			case 's': server	= std::string(optarg); break;
			case 'p': port		= std::string(optarg); break;
			case 'S': con_str += "--SERVER=" + std::string(optarg) + " "; break;
			case 't': num_threads	= strtol(optarg, NULL, 10); break;
			case 'n': num_ops	= strtol(optarg, NULL, 10); break;
			case 'o': num_objects	= strtol(optarg, NULL, 10); break;
			case 'k': size_objects	= strtol(optarg, NULL, 10); break;
			case 'd': size_dist	= strtof(optarg, NULL); break;
			case 'i': ratio_ops	= strtof(optarg, NULL); break; 
			case 'h': disp_help	= true;	
				usage(name);
		    }
		}
		con_str += "--SERVER=" + server + ":" + port;
	}

	void makeExecLine(char** line, int len) {
		*line = new char[len];
		//for running memcached locally.
		strcpy(*line, buildPath.c_str());
		strncat(*line, " -p ", len - strlen(*line));
		strncat(*line, port.c_str(), len - strlen(*line));
		if(run_server_binary) strncat(*line, " -B binary ", len - strlen(*line));
	}

};
