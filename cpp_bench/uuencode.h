#pragma once

#include <string.h>
#include <cstdlib>
#include <cmath>

/*
 * uuencode
 * function for performing uuencoding on a given string. The result of the encoding
 * is stored in allocated array res with maximum buffer size buff_size. The result
 * is truncated if it exceeds the buffer size of the result. Result must be freed
 * by caller.
 * @param res (char*) an unallocated character array for the result
 * @param buff_size (size_t) the maximum output size
 * @param src (char*) the source character array to be encoded
 * @param size (size_t) the number of characters in src to be encoded
 * @return (int)
 */
int uuencode(char** res_p, const char* src, size_t size) {
	int res_size = ceil((size*4)/3.0) + 1;
	*res_p = (char*) malloc(res_size); //4 encoded bytes per 
	char* res = *res_p;	
	int src_pos = 0;
	int res_pos = 0;
	char* e;
	unsigned char u[3];
	while(src_pos < size) {
		//grab three unencoded bytes
		u[0] = src[src_pos];
		u[1] = src[src_pos + 1];
		u[2] = src[src_pos + 2];
		//correct for over shoot
		for(int i = (size-src_pos); i < 3; ++i) {
			u[i] = '\0';
		}

		e = &(res[res_pos]);
		//u == next three unencoded bytes, e == start of next four bytes for result	
		e[0] = (u[0] >> 2) & 0x3f; 
		e[1] = ((u[0] << 4) | (u[1] >> 4) & 0x0f) & 0x3f;
		e[2] = ((u[1] << 2) | (u[2] >> 6) & 0x03) & 0x3f;
		e[3] = (u[2]) & 0x3f;
		
		for(int i = 0; i < 4; ++i) {
			e[i] += ' ';
			if(e[i] == ' ') {
				e[i] = '`';
			}
		}	
	
		src_pos += 3; //move forward three bytes
		res_pos += 4;
	}
	res[res_size-1] = '\0';

	return res_size;
}


/*
* uudecode
* function for performing uudecoding on a given string. The result of the decoding
* is stored in allocated array res with maximum buffer size buff_size. Result must
* be freed by the caller.
* @param res (char*) an unallocated character array for the result
* @param buff_size (size_t) the maximum output size
* @param src (char*) the source character array to be encoded
* @param size (size_t) the number of characters in src to be encoded
*/
int uudecode(char** res_p, const char* src, size_t size) {
	int res_size = ceil((size*3)/4.0) ;
	*res_p = (char*) malloc(res_size);
	char* res = *res_p;

	int src_pos = 0;
	int res_pos = 0;
	char* u;
	unsigned char e[4];
	while(src_pos < size) {
		//grab three unencoded bytes
		e[0] = src[src_pos];
		e[1] = src[src_pos + 1];
		e[2] = src[src_pos + 2];
		e[3] = src[src_pos + 3];
		//correct for over shoot
		for(int i = (size-src_pos); i < 4; ++i) {
			e[i] = '\0';
		}
		
		u = &(res[res_pos]);
		//u == next three unencoded bytes, e == start of next four bytes for result	
		u[0] = ((e[0]-' ') << 2) + (((e[1]-' ')&0x30) >> 4); 
		u[1] = (((e[1]-' ')&0x0f) << 4) + (((e[2]-' ')&0x3c) >> 2); 
		u[2] = (((e[2]-' ')&0x03) << 6) + ((e[3]-' ')&0x3f);
		
		src_pos += 4; //move forward four bytes
		res_pos += 3;
	}
	res[res_size-1] = '\0';

	return res_size;
}
