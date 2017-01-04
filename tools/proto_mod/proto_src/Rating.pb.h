// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Rating.proto

#ifndef PROTOBUF_Rating_2eproto__INCLUDED
#define PROTOBUF_Rating_2eproto__INCLUDED

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
#include "AiringType.pb.h"
// @@protoc_insertion_point(includes)

namespace scheduler {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Rating_2eproto();
void protobuf_AssignDesc_Rating_2eproto();
void protobuf_ShutdownFile_Rating_2eproto();

class Rating;

// ===================================================================

class Rating : public ::google::protobuf::Message {
 public:
  Rating();
  virtual ~Rating();
  
  Rating(const Rating& from);
  
  inline Rating& operator=(const Rating& from) {
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
  static const Rating& default_instance();
  
  void Swap(Rating* other);
  
  // implements Message ----------------------------------------------
  
  Rating* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Rating& from);
  void MergeFrom(const Rating& from);
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
  
  // optional string scheme = 1;
  inline bool has_scheme() const;
  inline void clear_scheme();
  static const int kSchemeFieldNumber = 1;
  inline const ::std::string& scheme() const;
  inline void set_scheme(const ::std::string& value);
  inline void set_scheme(const char* value);
  inline void set_scheme(const char* value, size_t size);
  inline ::std::string* mutable_scheme();
  inline ::std::string* release_scheme();
  
  // optional string rating = 2;
  inline bool has_rating() const;
  inline void clear_rating();
  static const int kRatingFieldNumber = 2;
  inline const ::std::string& rating() const;
  inline void set_rating(const ::std::string& value);
  inline void set_rating(const char* value);
  inline void set_rating(const char* value, size_t size);
  inline ::std::string* mutable_rating();
  inline ::std::string* release_rating();
  
  // repeated string subRatings = 3;
  inline int subratings_size() const;
  inline void clear_subratings();
  static const int kSubRatingsFieldNumber = 3;
  inline const ::std::string& subratings(int index) const;
  inline ::std::string* mutable_subratings(int index);
  inline void set_subratings(int index, const ::std::string& value);
  inline void set_subratings(int index, const char* value);
  inline void set_subratings(int index, const char* value, size_t size);
  inline ::std::string* add_subratings();
  inline void add_subratings(const ::std::string& value);
  inline void add_subratings(const char* value);
  inline void add_subratings(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& subratings() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_subratings();
  
  // @@protoc_insertion_point(class_scope:scheduler.Rating)
 private:
  inline void set_has_scheme();
  inline void clear_has_scheme();
  inline void set_has_rating();
  inline void clear_has_rating();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* scheme_;
  ::std::string* rating_;
  ::google::protobuf::RepeatedPtrField< ::std::string> subratings_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_Rating_2eproto();
  friend void protobuf_AssignDesc_Rating_2eproto();
  friend void protobuf_ShutdownFile_Rating_2eproto();
  
  void InitAsDefaultInstance();
  static Rating* default_instance_;
};
// ===================================================================


// ===================================================================

// Rating

// optional string scheme = 1;
inline bool Rating::has_scheme() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Rating::set_has_scheme() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Rating::clear_has_scheme() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Rating::clear_scheme() {
  if (scheme_ != &::google::protobuf::internal::kEmptyString) {
    scheme_->clear();
  }
  clear_has_scheme();
}
inline const ::std::string& Rating::scheme() const {
  return *scheme_;
}
inline void Rating::set_scheme(const ::std::string& value) {
  set_has_scheme();
  if (scheme_ == &::google::protobuf::internal::kEmptyString) {
    scheme_ = new ::std::string;
  }
  scheme_->assign(value);
}
inline void Rating::set_scheme(const char* value) {
  set_has_scheme();
  if (scheme_ == &::google::protobuf::internal::kEmptyString) {
    scheme_ = new ::std::string;
  }
  scheme_->assign(value);
}
inline void Rating::set_scheme(const char* value, size_t size) {
  set_has_scheme();
  if (scheme_ == &::google::protobuf::internal::kEmptyString) {
    scheme_ = new ::std::string;
  }
  scheme_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Rating::mutable_scheme() {
  set_has_scheme();
  if (scheme_ == &::google::protobuf::internal::kEmptyString) {
    scheme_ = new ::std::string;
  }
  return scheme_;
}
inline ::std::string* Rating::release_scheme() {
  clear_has_scheme();
  if (scheme_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = scheme_;
    scheme_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string rating = 2;
inline bool Rating::has_rating() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Rating::set_has_rating() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Rating::clear_has_rating() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Rating::clear_rating() {
  if (rating_ != &::google::protobuf::internal::kEmptyString) {
    rating_->clear();
  }
  clear_has_rating();
}
inline const ::std::string& Rating::rating() const {
  return *rating_;
}
inline void Rating::set_rating(const ::std::string& value) {
  set_has_rating();
  if (rating_ == &::google::protobuf::internal::kEmptyString) {
    rating_ = new ::std::string;
  }
  rating_->assign(value);
}
inline void Rating::set_rating(const char* value) {
  set_has_rating();
  if (rating_ == &::google::protobuf::internal::kEmptyString) {
    rating_ = new ::std::string;
  }
  rating_->assign(value);
}
inline void Rating::set_rating(const char* value, size_t size) {
  set_has_rating();
  if (rating_ == &::google::protobuf::internal::kEmptyString) {
    rating_ = new ::std::string;
  }
  rating_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Rating::mutable_rating() {
  set_has_rating();
  if (rating_ == &::google::protobuf::internal::kEmptyString) {
    rating_ = new ::std::string;
  }
  return rating_;
}
inline ::std::string* Rating::release_rating() {
  clear_has_rating();
  if (rating_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = rating_;
    rating_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated string subRatings = 3;
inline int Rating::subratings_size() const {
  return subratings_.size();
}
inline void Rating::clear_subratings() {
  subratings_.Clear();
}
inline const ::std::string& Rating::subratings(int index) const {
  return subratings_.Get(index);
}
inline ::std::string* Rating::mutable_subratings(int index) {
  return subratings_.Mutable(index);
}
inline void Rating::set_subratings(int index, const ::std::string& value) {
  subratings_.Mutable(index)->assign(value);
}
inline void Rating::set_subratings(int index, const char* value) {
  subratings_.Mutable(index)->assign(value);
}
inline void Rating::set_subratings(int index, const char* value, size_t size) {
  subratings_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Rating::add_subratings() {
  return subratings_.Add();
}
inline void Rating::add_subratings(const ::std::string& value) {
  subratings_.Add()->assign(value);
}
inline void Rating::add_subratings(const char* value) {
  subratings_.Add()->assign(value);
}
inline void Rating::add_subratings(const char* value, size_t size) {
  subratings_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
Rating::subratings() const {
  return subratings_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
Rating::mutable_subratings() {
  return &subratings_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace scheduler

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Rating_2eproto__INCLUDED
