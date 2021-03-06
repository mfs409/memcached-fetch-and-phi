// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AiringType.proto

#ifndef PROTOBUF_AiringType_2eproto__INCLUDED
#define PROTOBUF_AiringType_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace scheduler {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_AiringType_2eproto();
void protobuf_AssignDesc_AiringType_2eproto();
void protobuf_ShutdownFile_AiringType_2eproto();


enum AiringType {
  New = 0,
  Live = 1,
  Repeat = 2,
  Taped = 3,
  Unknown = 4
};
bool AiringType_IsValid(int value);
const AiringType AiringType_MIN = New;
const AiringType AiringType_MAX = Unknown;
const int AiringType_ARRAYSIZE = AiringType_MAX + 1;

const ::google::protobuf::EnumDescriptor* AiringType_descriptor();
inline const ::std::string& AiringType_Name(AiringType value) {
  return ::google::protobuf::internal::NameOfEnum(
    AiringType_descriptor(), value);
}
inline bool AiringType_Parse(
    const ::std::string& name, AiringType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<AiringType>(
    AiringType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

}  // namespace scheduler

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< scheduler::AiringType>() {
  return scheduler::AiringType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_AiringType_2eproto__INCLUDED
