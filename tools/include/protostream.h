#pragma once

#include <iostream>

class protostream : public std::iostream, public std::streambuf {
public:
	protostream(const char* beg, const char* end):std::iostream(this) {
		this->setg(const_cast<char *>(beg), const_cast<char *>(beg), const_cast<char *>(end));
	}	
};
