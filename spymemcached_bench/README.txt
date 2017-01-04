Spymemcached

/test: Test files for running spymemcached client with memcached server
	/test/Makefile: two commands
		make test - compile Test.java
		make run -compile and run Test.java

	/test/build_memcached/bin: 
		memcached - run with ./memcached
		strfcn.so - functions for basic string modification
		protolib.so - functions for modifying comcast protocol buffers

/spymemcached: Source for spymemcached implementation
	run 'ant' to compile. Jars located at /build/jars
