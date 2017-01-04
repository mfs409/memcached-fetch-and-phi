// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test.proto

#ifndef PROTOBUF_test_2eproto__INCLUDED
#define PROTOBUF_test_2eproto__INCLUDED

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

namespace test {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_test_2eproto();
void protobuf_AssignDesc_test_2eproto();
void protobuf_ShutdownFile_test_2eproto();

class Entry;

// ===================================================================

class Entry : public ::google::protobuf::Message {
 public:
  Entry();
  virtual ~Entry();
  
  Entry(const Entry& from);
  
  inline Entry& operator=(const Entry& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Entry& default_instance();
  
  void Swap(Entry* other);
  
  // implements Message ----------------------------------------------
  
  Entry* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Entry& from);
  void MergeFrom(const Entry& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional fixed32 data = 1;
  inline bool has_data() const;
  inline void clear_data();
  static const int kDataFieldNumber = 1;
  inline ::google::protobuf::uint32 data() const;
  inline void set_data(::google::protobuf::uint32 value);
  
  // repeated fixed32 arr = 2;
  inline int arr_size() const;
  inline void clear_arr();
  static const int kArrFieldNumber = 2;
  inline ::google::protobuf::uint32 arr(int index) const;
  inline void set_arr(int index, ::google::protobuf::uint32 value);
  inline void add_arr(::google::protobuf::uint32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      arr() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_arr();
  
  // optional string mute = 3;
  inline bool has_mute() const;
  inline void clear_mute();
  static const int kMuteFieldNumber = 3;
  inline const ::std::string& mute() const;
  inline void set_mute(const ::std::string& value);
  inline void set_mute(const char* value);
  inline void set_mute(const char* value, size_t size);
  inline ::std::string* mutable_mute();
  inline ::std::string* release_mute();
  
  // @@protoc_insertion_point(class_scope:test.Entry)
 private:
  inline void set_has_data();
  inline void clear_has_data();
  inline void set_has_mute();
  inline void clear_has_mute();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > arr_;
  ::std::string* mute_;
  ::google::protobuf::uint32 data_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_test_2eproto();
  friend void protobuf_AssignDesc_test_2eproto();
  friend void protobuf_ShutdownFile_test_2eproto();
  
  void InitAsDefaultInstance();
  static Entry* default_instance_;
};
// ===================================================================


// ===================================================================

// Entry

// optional fixed32 data = 1;
inline bool Entry::has_data() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Entry::set_has_data() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Entry::clear_has_data() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Entry::clear_data() {
  data_ = 0u;
  clear_has_data();
}
inline ::google::protobuf::uint32 Entry::data() const {
  return data_;
}
inline void Entry::set_data(::google::protobuf::uint32 value) {
  set_has_data();
  data_ = value;
}

// repeated fixed32 arr = 2;
inline int Entry::arr_size() const {
  return arr_.size();
}
inline void Entry::clear_arr() {
  arr_.Clear();
}
inline ::google::protobuf::uint32 Entry::arr(int index) const {
  return arr_.Get(index);
}
inline void Entry::set_arr(int index, ::google::protobuf::uint32 value) {
  arr_.Set(index, value);
}
inline void Entry::add_arr(::google::protobuf::uint32 value) {
  arr_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
Entry::arr() const {
  return arr_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
Entry::mutable_arr() {
  return &arr_;
}

// optional string mute = 3;
inline bool Entry::has_mute() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Entry::set_has_mute() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Entry::clear_has_mute() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Entry::clear_mute() {
  if (mute_ != &::google::protobuf::internal::kEmptyString) {
    mute_->clear();
  }
  clear_has_mute();
}
inline const ::std::string& Entry::mute() const {
  return *mute_;
}
inline void Entry::set_mute(const ::std::string& value) {
  set_has_mute();
  if (mute_ == &::google::protobuf::internal::kEmptyString) {
    mute_ = new ::std::string;
  }
  mute_->assign(value);
}
inline void Entry::set_mute(const char* value) {
  set_has_mute();
  if (mute_ == &::google::protobuf::internal::kEmptyString) {
    mute_ = new ::std::string;
  }
  mute_->assign(value);
}
inline void Entry::set_mute(const char* value, size_t size) {
  set_has_mute();
  if (mute_ == &::google::protobuf::internal::kEmptyString) {
    mute_ = new ::std::string;
  }
  mute_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Entry::mutable_mute() {
  set_has_mute();
  if (mute_ == &::google::protobuf::internal::kEmptyString) {
    mute_ = new ::std::string;
  }
  return mute_;
}
inline ::std::string* Entry::release_mute() {
  clear_has_mute();
  if (mute_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = mute_;
    mute_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace test

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_test_2eproto__INCLUDED
