#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <gzstream.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <stdlib.h>
#include "timing.h"
#include "protostream.h"
#include "combo.pb.h"
#include "repeat_int.pb.h"
#include "string.pb.h"

#define SIZE_THRESH 16384
#define MAX_SIZE 1024*1024 //1MB
#define	STEP 1024 //1KB steps

const int MAX_BUF = 1024*1025;
const int size_packet_len = 10;
const char *write_file = "out.gz";
const char *write_txt_file = "out.txt";
const char *filler_text = "les_mis.txt";
static const char char_list[] = "ABCDEFGHIJLKMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\t\r\n ";
/**Used to generate numbers for repeating int fields**/
typedef struct file_stats file_stats;
struct file_stats {
	long before;
	long after;
	long compress_time;
	long decompress_time;
	long trans_time;
	int num_ints;
	int num_bytes;
};

long long randInt(long long min, long long max) {
	return rand()%(max-min+1) + min;
}

long getFileSize(const char *file) {
	struct stat fileStat;
	stat(file, &fileStat);
	return (long)fileStat.st_size;
}

char randChar(const char *list) {
	return list[rand()%(strlen(list))];
}

std::string zeroPad(int num, int len) {
	std::string res;
	std::ostringstream sout(res);
	sout << num;
	res = sout.str();
	while(res.length() < len) {
		res = "0" + res;
	}
	return res;
}

int openSocket(int argc, char **argv) {
	struct addrinfo hints, *server_info, *addr_iter;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	int rv;
	if((rv = getaddrinfo(argv[1], argv[2], &hints, &server_info)) != 0) {
		printf("Couldn't get addrinfo for <%s>:<%s>\n", argv[1], argv[2]);
		return -1;
	}

	int socket_id = -1;
	for(addr_iter = server_info; addr_iter != NULL; addr_iter = addr_iter->ai_next) {
		if((socket_id = socket(addr_iter->ai_family, addr_iter->ai_socktype, 0)) == -1) {
			perror("Client: socket");
			continue;
		}
		if(connect(socket_id, addr_iter->ai_addr, addr_iter->ai_addrlen) == -1) {
			perror("Client: connect");
			socket_id = -1;
			continue;
		}
		break;
	}
	return socket_id;
}

void fillString(std::string &str, int num_bytes) {
	std::ifstream fin(filler_text);
	std::string temp;
	while(str.length() < num_bytes) {
		getline(fin, temp);
		str += temp;
	}
	fin.close();
}

int main( int argc, char*argv[]) {
/*
	std::string line;
	while ( getline(in, line)) {
		out << line << std::endl;
	}
	in.close();
	out.close();
	std::cout<<"Compressed size: "<<getFileSize(argv[2])<<std::endl;
	if ( ! in.eof()) {
		std::cerr << "ERROR: Reading file `" << argv[1] << "' failed.\n";
		return EXIT_FAILURE;
	}
	if ( ! out.good()) {
		std::cerr << "ERROR: Writing file `" << argv[2] << "' failed.\n";
		return EXIT_FAILURE;
	}
*/
	int sock_id = openSocket(argc, argv);
	if(sock_id == -1) {
		std::cerr << "Couldn't open socket at " << argv[1] <<":"<< argv[2] <<std::endl;
		return -1;
	}
	char *buf = (char*)malloc(MAX_BUF);

	file_stats stats;
	std::ofstream txt_out;
	for(int num_tests = 0; num_tests < 5; num_tests++) {
		std::ostringstream sin;
		sin << "compression_stats_" << num_tests << ".txt";
		std::ofstream stats_out(sin.str().c_str());
		stats_out << "TARGET_SIZE RAW COMPRESSED PERCENT NUM_INT64 NUM_STR C_TIME D_TIME T_TIME\n";	
		for(long i = STEP; i < MAX_SIZE; i += STEP) {
			std::cout<<i/STEP<<std::endl;
			for(int a = 0; a <= 100; a++) {
				test::Combo c;
				int num_bytes = i*(100-a)/100.0;
				int num_ints = i*(a)/100.0;
				stats.num_ints = num_ints;
				stats.num_bytes = num_bytes;
				num_ints /= sizeof(unsigned long long);
				while(num_ints-- > 0) {
					c.add_i(randInt(0, LLONG_MAX));
				}
				std::string filler = "";
				fillString(filler, num_bytes);
				c.set_str(filler);
				std::string out;
				c.SerializeToString(&out);

				//TEST: compression time
				stats.before = out.length();
				ogzstream gz_out;
				gz_out.open(write_file);
				while(!gz_out.good()) {
					std::cout << "Retrying out open" << std::endl;
					gz_out.close();
					gz_out.open(write_file);
				}
				stats.compress_time = getElapsedTime();
				gz_out << out;
				gz_out.close();
				stats.compress_time = getElapsedTime() - stats.compress_time;

				//TEST: decompression time		
				//this isn't opening correctly. Why?	
				//Had to move igzstream declaration here
				igzstream gz_in;
				stats.decompress_time = getElapsedTime();
				gz_in.open(write_file);
				while(!gz_in.good()) {
					std::cout << "Retrying in open" << std::endl;
					gz_in.close();
					gz_in.open(write_file);
					return -1;
				}
				std::string i_str;
				std::ofstream fout(write_txt_file);
				while(getline(gz_in, i_str));
				fout.close();
				gz_in.close();
				stats.decompress_time = getElapsedTime() - stats.decompress_time;

				//TEST: compressed size
				stats.after = getFileSize(write_file);

				//TEST: socket transmit round trip
				//I time this entire process to roughly simulate some computation
				// in preparing packets
				stats.trans_time = getElapsedTime();	
				std::string send_size = zeroPad(out.length(), size_packet_len);	
				ssize_t s = send(sock_id, send_size.c_str(), size_packet_len, 0);
				s = send(sock_id, out.c_str(), out.length(), 0);	

				while((s = recv(sock_id, buf, 10, 0)) <= 0);
				buf[s] = '\0';
				long size = strtol(buf, NULL, 10);
				while(size > 0) {
					int num_recv = recv(sock_id, buf, MAX_BUF-1, 0);
					size -= num_recv;
				}
				stats.trans_time = getElapsedTime() - stats.trans_time;
		
				stats_out << i << " " << stats.before << " " << stats.after << " " << (1 - (float)stats.after/stats.before) << " " << stats.num_ints << " " << stats.num_bytes << " " << stats.compress_time << " " << stats.decompress_time << " " << stats.trans_time << std::endl;
			}
		}	
	}

	close(sock_id);
	return EXIT_SUCCESS;
}
/*
void retrieveTest() {
	memcached_return_t rc;
	size_t vSize;
	uint32_t flags;

	char* val = memcached_get(memc, key.c_str(), key.length(), &vSize, &flags, &rc);

	protostream pin(val, val+vSize);

	test::Entry e;
	if(!e.ParseFromIstream(&pin)) {
		cout << "retrieveTest failed..." << endl;
	}
	else if(verifyTest(i_test, e)) {
		cout << "It looks right." << endl;
	}
	else {
		cout << "WRONG" << endl;
	}
}
*/
