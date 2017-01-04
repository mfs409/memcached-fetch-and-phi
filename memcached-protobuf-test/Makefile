#flags for compiling protobuf library from local source
PROTOBUF_FLAGS = -I../protobuf-2.4.1/src -L../protobuf-2.4.1/src/.libs -lprotobuf

all: mcpb_test_cpp connect

protoc_comp: 
	protoc --cpp_out=. test.proto

mcpb_test_cpp: test.cc protoc_comp
	c++ test.cc test.pb.cc -o test -D_THREAD_SAFE $(PROTOBUF_FLAGS) -lmemcached -lpthread 
connect: connect.cc
	c++ connect.cc -o connect -lmemcached
