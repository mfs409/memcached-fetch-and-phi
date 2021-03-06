// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: repeat_int.proto

#ifndef PROTOBUF_repeat_5fint_2eproto__INCLUDED
#define PROTOBUF_repeat_5fint_2eproto__INCLUDED

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
void  protobuf_AddDesc_repeat_5fint_2eproto();
void protobuf_AssignDesc_repeat_5fint_2eproto();
void protobuf_ShutdownFile_repeat_5fint_2eproto();

class Ints;

// ===================================================================

class Ints : public ::google::protobuf::Message {
 public:
  Ints();
  virtual ~Ints();
  
  Ints(const Ints& from);
  
  inline Ints& operator=(const Ints& from) {
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
  static const Ints& default_instance();
  
  void Swap(Ints* other);
  
  // implements Message ----------------------------------------------
  
  Ints* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Ints& from);
  void MergeFrom(const Ints& from);
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
  
  // @@protoc_insertion_point(class_scope:test.Ints)
 private:
  inline void set_has_data();
  inline void clear_has_data();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > arr_;
  ::google::protobuf::uint32 data_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_repeat_5fint_2eproto();
  friend void protobuf_AssignDesc_repeat_5fint_2eproto();
  friend void protobuf_ShutdownFile_repeat_5fint_2eproto();
  
  void InitAsDefaultInstance();
  static Ints* default_instance_;
};
// ===================================================================


// ===================================================================

// Ints

// optional fixed32 data = 1;
inline bool Ints::has_data() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Ints::set_has_data() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Ints::clear_has_data() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Ints::clear_data() {
  data_ = 0u;
  clear_has_data();
}
inline ::google::protobuf::uint32 Ints::data() const {
  return data_;
}
inline void Ints::set_data(::google::protobuf::uint32 value) {
  set_has_data();
  data_ = value;
}

// repeated fixed32 arr = 2;
inline int Ints::arr_size() const {
  return arr_.size();
}
inline void Ints::clear_arr() {
  arr_.Clear();
}
inline ::google::protobuf::uint32 Ints::arr(int index) const {
  return arr_.Get(index);
}
inline void Ints::set_arr(int index, ::google::protobuf::uint32 value) {
  arr_.Set(index, value);
}
inline void Ints::add_arr(::google::protobuf::uint32 value) {
  arr_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
Ints::arr() const {
  return arr_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
Ints::mutable_arr() {
  return &arr_;
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

#endif  // PROTOBUF_repeat_5fint_2eproto__INCLUDED
