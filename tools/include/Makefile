all: fcn_map shared

fcn_map: fcn_map.c fcn_map.h
	c++ -c fcn_map.c

shared: strfcn.cc
	c++ -fPIC -shared strfcn.cc -o strfcn.so
