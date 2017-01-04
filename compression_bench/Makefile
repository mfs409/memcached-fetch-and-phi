CXX      = g++   # for Linux RedHat 6.1, g++ version 2.95.2

CPPFLAGS = -std=c++11 -I../gzstream 
LDFLAGS  = -L../gzstream -lgzstream -lz
PROTOBUF_FLAGS = -I../tools/include -I../protobuf-2.4.1/src -I./protobufs -L../protobuf-2.4.1/src/.libs -lprotobuf
PROTOBUF_SRC = protobufs/repeat_int.pb.cc protobufs/string.pb.cc protobufs/combo.pb.cc

all: bench server

bench : bench.cc
	${CXX} ${CPPFLAGS} bench.cc $(PROTOBUF_SRC) -o bench -D_THREAD_SAFE $(PROTOBUF_FLAGS) -lpthread ${LDFLAGS}

server: bench_server.cc
	${CXX} ${CPPFLAGS} bench_server.cc -o server

clean :
	rm *.o
