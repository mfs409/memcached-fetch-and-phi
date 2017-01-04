all: protobuf_test_cpp

protoc_comp: 
	protoc --cpp_out=. test.proto

protobuf_test_cpp: update_addressbook.cc protoc_comp
	c++ update_addressbook.cc test.pb.cc -o update_addresses -I/home/ams314/protobuf-2.5.0/src -L/home/ams314/protobuf -lprotobuf
