PROTOBUF_FLAGS =  -Iproto_src/ -Iproto_src/cache -I../../../protobuf-2.4.1/src -I../include/ -I../../include -L../../../protobuf-2.4.1/src/.libs
PROTOBUF_SRC = proto_src/AiringType.pb.cc proto_src/ChannelOptions.pb.cc proto_src/DeletePriority.pb.cc proto_src/EpisodeOptions.pb.cc proto_src/FrequencyOptions.pb.cc proto_src/ListingData.pb.cc proto_src/MinimalListingData.pb.cc proto_src/MinimalRecording.pb.cc proto_src/NotRecordReason.pb.cc proto_src/Rating.pb.cc proto_src/Recording.pb.cc proto_src/RecordingOptions.pb.cc proto_src/RecordingSegment.pb.cc proto_src/RecordingStatus.pb.cc proto_src/RecordingType.pb.cc proto_src/cache/MinimalRecordingList.pb.cc proto_src/cache/RecordingList.pb.cc

all: test shared

test: test.cc 
	c++ test.cc -o test $(PROTOBUF_FLAGS) -lprotobuf -lpthread -ldl
shared: proto_mod.c
	c++ -fPIC -shared proto_mod.c $(PROTOBUF_SRC) -o protolib.so $(PROTOBUF_FLAGS) -lprotobuf -lpthread
	cp protolib.so ../include/
