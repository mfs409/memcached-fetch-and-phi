# Memcached + Fetch-and-Phi
Here's everything cleaned up a little bit. The code is fairly sloppy.
A rundown of what there is and where to find it:

-Compression Bench:
	=dir compression_bench
	=A time comparision for compressing, decompressing, and sending
	 objects over a network connection. Two executables, one client
	 , one server. Uses a mix of text and numerical data in a 
	 protobuf.

-Cpp Bench:
	=dir cpp_bench
	=A driver for testing the first iteration of fetch-and-phi. Written
	 in cpp using libmemcached, modified to process register and invoke
	 operations.
	=options?

-Spymemcached Benchs:
	=dir spymemcached_bench
	=Contains modified Spymemcached source for client register and invoke.

	=dir spymemcached_bench/test
	=The various test benches written in java. See the readme here for
	 what each test does. 

-Memcached:
	=dir memcached-1.4.20
	=memcached.c contains most of the modifications. See comments within
	 the file to get a better understanding of how everything is cobbled
	 together. `ams314` marks comments that I have added
	=Define PAPI while making to enable performance tracking. Papi options
	 are located at the top of memcached.c
	=The build_memcached script in the top level directory will configure
	 and build the memcached source in a specified directory. You may also
	 run `make && make install` in the source directory.

-Tools:
	=dir tools/proto_mod
	=All the source code for comcast's protocol buffers, and several
	 functions for manipulating protocol buffers in the cache. 
