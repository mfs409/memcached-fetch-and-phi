all: uuencode

uuencode: uuencode_test.cc uuencode.h
	c++ uuencode_test.cc -o uuencode_test
