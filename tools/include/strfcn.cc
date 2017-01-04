#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {

	//simple function to change a string to all uppercase
	int toupper_str(char* b, int n, int MAX_SIZE) {
		int diff = 'a' - 'A';
		//yeah it isn't very safe
		for(char* ptr = b; ptr < b+n; ptr++) {
			if(*ptr >= 'a' && *ptr <= 'z') {
				char temp = *ptr - diff;
				*ptr = temp;
			}
		}

		return n;
	}

	int tolower_str(char* b, int n, int MAX_SIZE) {
		int diff = 'a' - 'A';
		//yeah it isn't very safe
		for(char* ptr = b; ptr < b+n; ptr++) {
			if(*ptr >= 'A' && *ptr <= 'Z') {
				char temp = *ptr + diff;
				*ptr = temp;
			}
		}

		return n;
	}

	int repeat10(char* b, int n, int MAX_SIZE) {
		if(n*10 > MAX_SIZE) return n;
		char *r = b;
		for(int i = 1; i < 10; ++i) {
			memcpy(r+n*i, b, n);
		}
		return n*10;
	}

	int truncate(char* b, int n, int MAX_SIZE) {
		return 10;
	}
}
