// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "RecordingSegment.pb.h"

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

const ::google::protobuf::Descriptor* RecordingSegment_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  RecordingSegment_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_RecordingSegment_2eproto() {
  protobuf_AddDesc_RecordingSegment_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "RecordingSegment.proto");
  GOOGLE_CHECK(file != NULL);
  RecordingSegment_descriptor_ = file->message_type(0);
  static const int RecordingSegment_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RecordingSegment, actualstart_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RecordingSegment, actualend_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RecordingSegment, segmentnumber_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RecordingSegment, volume_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RecordingSegment, playbacklocator_),
  };
  RecordingSegment_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      RecordingSegment_descriptor_,
      RecordingSegment::default_instance_,
      RecordingSegment_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RecordingSegment, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RecordingSegment, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(RecordingSegment));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_RecordingSegment_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    RecordingSegment_descriptor_, &RecordingSegment::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_RecordingSegment_2eproto() {
  delete RecordingSegment::default_instance_;
  delete RecordingSegment_reflection_;
}

void protobuf_AddDesc_RecordingSegment_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026RecordingSegment.proto\022\tscheduler\"z\n\020R"
    "ecordingSegment\022\023\n\013actualStart\030\001 \002(\003\022\021\n\t"
    "actualEnd\030\002 \002(\003\022\025\n\rsegmentNumber\030\003 \002(\005\022\016"
    "\n\006volume\030\004 \001(\t\022\027\n\017playBackLocator\030\005 \001(\tB"
    "D\n+com.comcast.data.protobuf.message.sch"
    "edulerB\025RecordingSegmentProto", 229);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "RecordingSegment.proto", &protobuf_RegisterTypes);
  RecordingSegment::default_instance_ = new RecordingSegment();
  RecordingSegment::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_RecordingSegment_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_RecordingSegment_2eproto {
  StaticDescriptorInitializer_RecordingSegment_2eproto() {
    protobuf_AddDesc_RecordingSegment_2eproto();
  }
} static_descriptor_initializer_RecordingSegment_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int RecordingSegment::kActualStartFieldNumber;
const int RecordingSegment::kActualEndFieldNumber;
const int RecordingSegment::kSegmentNumberFieldNumber;
const int RecordingSegment::kVolumeFieldNumber;
const int RecordingSegment::kPlayBackLocatorFieldNumber;
#endif  // !_MSC_VER

RecordingSegment::RecordingSegment()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void RecordingSegment::InitAsDefaultInstance() {
}

RecordingSegment::RecordingSegment(const RecordingSegment& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void RecordingSegment::SharedCtor() {
  _cached_size_ = 0;
  actualstart_ = GOOGLE_LONGLONG(0);
  actualend_ = GOOGLE_LONGLONG(0);
  segmentnumber_ = 0;
  volume_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  playbacklocator_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RecordingSegment::~RecordingSegment() {
  SharedDtor();
}

void RecordingSegment::SharedDtor() {
  if (volume_ != &::google::protobuf::internal::kEmptyString) {
    delete volume_;
  }
  if (playbacklocator_ != &::google::protobuf::internal::kEmptyString) {
    delete playbacklocator_;
  }
  if (this != default_instance_) {
  }
}

void RecordingSegment::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* RecordingSegment::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return RecordingSegment_descriptor_;
}

const RecordingSegment& RecordingSegment::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_RecordingSegment_2eproto();  return *default_instance_;
}

RecordingSegment* RecordingSegment::default_instance_ = NULL;

RecordingSegment* RecordingSegment::New() const {
  return new RecordingSegment;
}

void RecordingSegment::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    actualstart_ = GOOGLE_LONGLONG(0);
    actualend_ = GOOGLE_LONGLONG(0);
    segmentnumber_ = 0;
    if (has_volume()) {
      if (volume_ != &::google::protobuf::internal::kEmptyString) {
        volume_->clear();
      }
    }
    if (has_playbacklocator()) {
      if (playbacklocator_ != &::google::protobuf::internal::kEmptyString) {
        playbacklocator_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool RecordingSegment::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int64 actualStart = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &actualstart_)));
          set_has_actualstart();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_actualEnd;
        break;
      }
      
      // required int64 actualEnd = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_actualEnd:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &actualend_)));
          set_has_actualend();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_segmentNumber;
        break;
      }
      
      // required int32 segmentNumber = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_segmentNumber:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &segmentnumber_)));
          set_has_segmentnumber();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_volume;
        break;
      }
      
      // optional string volume = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_volume:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_volume()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->volume().data(), this->volume().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_playBackLocator;
        break;
      }
      
      // optional string playBackLocator = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_playBackLocator:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_playbacklocator()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->playbacklocator().data(), this->playbacklocator().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void RecordingSegment::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int64 actualStart = 1;
  if (has_actualstart()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->actualstart(), output);
  }
  
  // required int64 actualEnd = 2;
  if (has_actualend()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->actualend(), output);
  }
  
  // required int32 segmentNumber = 3;
  if (has_segmentnumber()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->segmentnumber(), output);
  }
  
  // optional string volume = 4;
  if (has_volume()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->volume().data(), this->volume().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->volume(), output);
  }
  
  // optional string playBackLocator = 5;
  if (has_playbacklocator()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->playbacklocator().data(), this->playbacklocator().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->playbacklocator(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* RecordingSegment::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int64 actualStart = 1;
  if (has_actualstart()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->actualstart(), target);
  }
  
  // required int64 actualEnd = 2;
  if (has_actualend()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->actualend(), target);
  }
  
  // required int32 segmentNumber = 3;
  if (has_segmentnumber()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->segmentnumber(), target);
  }
  
  // optional string volume = 4;
  if (has_volume()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->volume().data(), this->volume().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->volume(), target);
  }
  
  // optional string playBackLocator = 5;
  if (has_playbacklocator()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->playbacklocator().data(), this->playbacklocator().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->playbacklocator(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int RecordingSegment::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int64 actualStart = 1;
    if (has_actualstart()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->actualstart());
    }
    
    // required int64 actualEnd = 2;
    if (has_actualend()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->actualend());
    }
    
    // required int32 segmentNumber = 3;
    if (has_segmentnumber()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->segmentnumber());
    }
    
    // optional string volume = 4;
    if (has_volume()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->volume());
    }
    
    // optional string playBackLocator = 5;
    if (has_playbacklocator()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->playbacklocator());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void RecordingSegment::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const RecordingSegment* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const RecordingSegment*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void RecordingSegment::MergeFrom(const RecordingSegment& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_actualstart()) {
      set_actualstart(from.actualstart());
    }
    if (from.has_actualend()) {
      set_actualend(from.actualend());
    }
    if (from.has_segmentnumber()) {
      set_segmentnumber(from.segmentnumber());
    }
    if (from.has_volume()) {
      set_volume(from.volume());
    }
    if (from.has_playbacklocator()) {
      set_playbacklocator(from.playbacklocator());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void RecordingSegment::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void RecordingSegment::CopyFrom(const RecordingSegment& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RecordingSegment::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;
  
  return true;
}

void RecordingSegment::Swap(RecordingSegment* other) {
  if (other != this) {
    std::swap(actualstart_, other->actualstart_);
    std::swap(actualend_, other->actualend_);
    std::swap(segmentnumber_, other->segmentnumber_);
    std::swap(volume_, other->volume_);
    std::swap(playbacklocator_, other->playbacklocator_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata RecordingSegment::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = RecordingSegment_descriptor_;
  metadata.reflection = RecordingSegment_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace scheduler

// @@protoc_insertion_point(global_scope)
