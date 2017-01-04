#
#  Flags for compiling protobuf library from local source
#
PROTOBUF_FLAGS = -I../protobuf-2.4.1/src -L../protobuf-2.4.1/src/.libs -lprotobuf

#
#  The main target
#
TARGET=string repeat_int combo

all: $(TARGET)

%.pb.cc %.pb.h: %.proto
	@echo Generating .pb.cc and .pb.h files for $<
	protoc --cpp_out=. $<

string: string.pb.cc string.pb.h
repeat_int: repeat_int.pb.cc repeat_int.pb.h
combo: combo.pb.cc combo.pb.h

%: %.cc %.pb.cc
	c++ $^ -o $@ -D_THREAD_SAFE $(PROTOBUF_FLAGS) -lpthread 

clean:
	rm -f $(TARGET) test.pb.cc test.pb.h
