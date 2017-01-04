#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>

const int SOCKET_DOMAIN = AF_INET;
const int SOCKET_TYPE = SOCK_STREAM;
const int size_packet_len = 10;
const int MAX_BUF = 1024*1025;

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

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "Must provide port number.\n");
		return -1;
	}
	int port = strtol(argv[argc-1], NULL, 10);
	if(errno != 0) {
		fprintf(stderr, "Could not parse port nuber %s. Quitting\n", argv[argc-1]);
	}
	printf("Port %d\n", port);	
	struct sockaddr_in addr;
	addr.sin_family = SOCKET_DOMAIN;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);


	int sock_id = socket(SOCKET_DOMAIN, SOCKET_TYPE, 0);
	if(sock_id == -1) {
		fprintf(stderr, "Could not open socket. Quitting. \n");
		return -1;
	}

	if(bind(sock_id, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
		fprintf(stderr, "Error binding socket. Quitting\n");
		return -1;
	}
	listen(sock_id, 3);

	int cli_id;
	socklen_t cli_sock_len = sizeof(addr); 

	while((cli_id = accept(sock_id, (struct sockaddr*) &addr, &cli_sock_len)) == -1) {
		fprintf(stderr, "Waiting to bind client\n");
	}

	char *buf = (char*)malloc(MAX_BUF);
	char *write_buf = (char*)malloc(MAX_BUF);

	int num_recv;
	while(true) {
		num_recv = recv(cli_id, buf, size_packet_len, 0);
		if(num_recv > 0) {
			buf[num_recv] = '\0';
			long num_bytes = strtol(buf, NULL, 10);
			long ret_bytes = num_bytes;
			char *curr = write_buf;
			while(num_bytes > 0) {
				num_recv = recv(cli_id, buf, num_bytes, 0);
				if(num_recv > 0) {
					buf[num_recv] = '\0';
					memcpy(curr, buf, num_recv);
					num_bytes -= num_recv;
					
				}
			}	
			write_buf[ret_bytes] = '\0';
			std::string ret = zeroPad(ret_bytes, size_packet_len);
			send(cli_id, ret.c_str(), size_packet_len, 0);
			send(cli_id, write_buf, ret_bytes, 0);
		}
	}

	close(sock_id);
	return 0;
}
