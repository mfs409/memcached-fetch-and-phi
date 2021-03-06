// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "RecordingType.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace scheduler {

namespace {

const ::google::protobuf::EnumDescriptor* RecordingType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_RecordingType_2eproto() {
  protobuf_AddDesc_RecordingType_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "RecordingType.proto");
  GOOGLE_CHECK(file != NULL);
  RecordingType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_RecordingType_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_RecordingType_2eproto() {
}

void protobuf_AddDesc_RecordingType_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\023RecordingType.proto\022\tscheduler*\225\001\n\rRec"
    "ordingType\022\020\n\014HotRecording\020\000\022\024\n\020OneTimeR"
    "ecording\020\001\022\023\n\017SeriesRecording\020\002\022\021\n\rAutoR"
    "ecording\020\003\022\027\n\023ChannelHotRecording\020\004\022\033\n\027C"
    "hannelOneTimeRecording\020\005B2\n\034com.hercules"
    ".serverdata.dataB\022RecordingTypeProto", 236);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "RecordingType.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_RecordingType_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_RecordingType_2eproto {
  StaticDescriptorInitializer_RecordingType_2eproto() {
    protobuf_AddDesc_RecordingType_2eproto();
  }
} static_descriptor_initializer_RecordingType_2eproto_;

const ::google::protobuf::EnumDescriptor* RecordingType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return RecordingType_descriptor_;
}
bool RecordingType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace scheduler

// @@protoc_insertion_point(global_scope)
