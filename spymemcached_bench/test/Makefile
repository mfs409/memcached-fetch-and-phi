CLASSPATH=.:spymemcached-2.11.4.jar:com:protobuf-java-2.4.1.jar

all: test

test: Test.java
	javac -cp '$(CLASSPATH)' Test.java

perf: PerfTest.java
	javac -cp '$(CLASSPATH)' PerfTest.java

spike: SpikeTest.java
	javac -cp '$(CLASSPATH)' SpikeTest.java

runs: SpikeTest.java
	javac -cp '$(CLASSPATH)' SpikeTest.java
	java -cp '$(CLASSPATH)' SpikeTest 6pac1.cse.lehigh.edu 11211

fetch: FetchTest.java
	javac -cp '$(CLASSPATH)' FetchTest.java

runf: FetchTest.java
	javac -cp '$(CLASSPATH)' FetchTest.java
	java -cp '$(CLASSPATH)' FetchTest 6pac1.cse.lehigh.edu 11211

runp: 
	java -cp '$(CLASSPATH)' PerfTest 6pac1.cse.lehigh.edu 11211

run: test
	java -cp '$(CLASSPATH)' Test localhost 11211

remote: test
	java -cp '$(CLASSPATH)' Test 6pac1.cse.lehigh.edu 11211

