MEMCACHE_FLAGS= -I../libmemcached-1.0.18/include -L../libmemcached-1.0.18/lib -lmemcached

connect: connect.cc
	g++ -std=c++11 connect.cc -o connect $(MEMCACHE_FLAGS) 
