PROTOBUF_FLAGS = -I../protobuf-2.4.1/src -I../tools/include -L../protobuf-2.4.1/src/.libs -lprotobuf
PROTOBUF_SRC = ../tools/include/test.pb.cc
MEMCACHE_FLAGS= -I../libmemcached-1.0.18/include -L../libmemcached-1.0.18/lib -lmemcached

all: driver

driver: driver.cc
	c++ -std=c++11 driver.cc $(PROTOBUF_SRC) -o driver -D_THREAD_SAFE $(PROTOBUF_FLAGS) -pthread $(MEMCACHE_FLAGS)
