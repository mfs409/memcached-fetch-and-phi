PROTO_INCLUDE=-Icache -I.
FLAGS=--cpp_out=../proto_src --proto_path=. --proto_path=cache
SOURCE = AiringType.proto ChannelOptions.proto DeletePriority.proto EpisodeOptions.proto FrequencyOptions.proto ListingData.proto MinimalListingData.proto MinimalRecording.proto NotRecordReason.proto Rating.proto Recording.proto RecordingOptions.proto RecordingSegment.proto RecordingStatus.proto RecordingType.proto cache/MinimalRecordingList.proto cache/RecordingList.proto

proto: 
	protoc $(FLAGS) $(PROTO_INCLUDE) $(SOURCE)

