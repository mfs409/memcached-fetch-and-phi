// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test.proto

package test;

public final class Test {
  private Test() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface EntryOrBuilder
      extends com.google.protobuf.MessageOrBuilder {
    
    // optional fixed32 data = 1;
    boolean hasData();
    int getData();
    
    // repeated fixed32 arr = 2;
    java.util.List<java.lang.Integer> getArrList();
    int getArrCount();
    int getArr(int index);
    
    // optional string mute = 3;
    boolean hasMute();
    String getMute();
  }
  public static final class Entry extends
      com.google.protobuf.GeneratedMessage
      implements EntryOrBuilder {
    // Use Entry.newBuilder() to construct.
    private Entry(Builder builder) {
      super(builder);
    }
    private Entry(boolean noInit) {}
    
    private static final Entry defaultInstance;
    public static Entry getDefaultInstance() {
      return defaultInstance;
    }
    
    public Entry getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return test.Test.internal_static_test_Entry_descriptor;
    }
    
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return test.Test.internal_static_test_Entry_fieldAccessorTable;
    }
    
    private int bitField0_;
    // optional fixed32 data = 1;
    public static final int DATA_FIELD_NUMBER = 1;
    private int data_;
    public boolean hasData() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    public int getData() {
      return data_;
    }
    
    // repeated fixed32 arr = 2;
    public static final int ARR_FIELD_NUMBER = 2;
    private java.util.List<java.lang.Integer> arr_;
    public java.util.List<java.lang.Integer>
        getArrList() {
      return arr_;
    }
    public int getArrCount() {
      return arr_.size();
    }
    public int getArr(int index) {
      return arr_.get(index);
    }
    
    // optional string mute = 3;
    public static final int MUTE_FIELD_NUMBER = 3;
    private java.lang.Object mute_;
    public boolean hasMute() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    public String getMute() {
      java.lang.Object ref = mute_;
      if (ref instanceof String) {
        return (String) ref;
      } else {
        com.google.protobuf.ByteString bs = 
            (com.google.protobuf.ByteString) ref;
        String s = bs.toStringUtf8();
        if (com.google.protobuf.Internal.isValidUtf8(bs)) {
          mute_ = s;
        }
        return s;
      }
    }
    private com.google.protobuf.ByteString getMuteBytes() {
      java.lang.Object ref = mute_;
      if (ref instanceof String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8((String) ref);
        mute_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }
    
    private void initFields() {
      data_ = 0;
      arr_ = java.util.Collections.emptyList();;
      mute_ = "";
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;
      
      memoizedIsInitialized = 1;
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeFixed32(1, data_);
      }
      for (int i = 0; i < arr_.size(); i++) {
        output.writeFixed32(2, arr_.get(i));
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeBytes(3, getMuteBytes());
      }
      getUnknownFields().writeTo(output);
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeFixed32Size(1, data_);
      }
      {
        int dataSize = 0;
        dataSize = 4 * getArrList().size();
        size += dataSize;
        size += 1 * getArrList().size();
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeBytesSize(3, getMuteBytes());
      }
      size += getUnknownFields().getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }
    
    private static final long serialVersionUID = 0L;
    @java.lang.Override
    protected java.lang.Object writeReplace()
        throws java.io.ObjectStreamException {
      return super.writeReplace();
    }
    
    public static test.Test.Entry parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static test.Test.Entry parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static test.Test.Entry parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static test.Test.Entry parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static test.Test.Entry parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static test.Test.Entry parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static test.Test.Entry parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static test.Test.Entry parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input, extensionRegistry)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static test.Test.Entry parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static test.Test.Entry parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(test.Test.Entry prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder>
       implements test.Test.EntryOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return test.Test.internal_static_test_Entry_descriptor;
      }
      
      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return test.Test.internal_static_test_Entry_fieldAccessorTable;
      }
      
      // Construct using test.Test.Entry.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }
      
      private Builder(BuilderParent parent) {
        super(parent);
        maybeForceBuilderInitialization();
      }
      private void maybeForceBuilderInitialization() {
        if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
        }
      }
      private static Builder create() {
        return new Builder();
      }
      
      public Builder clear() {
        super.clear();
        data_ = 0;
        bitField0_ = (bitField0_ & ~0x00000001);
        arr_ = java.util.Collections.emptyList();;
        bitField0_ = (bitField0_ & ~0x00000002);
        mute_ = "";
        bitField0_ = (bitField0_ & ~0x00000004);
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }
      
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return test.Test.Entry.getDescriptor();
      }
      
      public test.Test.Entry getDefaultInstanceForType() {
        return test.Test.Entry.getDefaultInstance();
      }
      
      public test.Test.Entry build() {
        test.Test.Entry result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }
      
      private test.Test.Entry buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        test.Test.Entry result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return result;
      }
      
      public test.Test.Entry buildPartial() {
        test.Test.Entry result = new test.Test.Entry(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.data_ = data_;
        if (((bitField0_ & 0x00000002) == 0x00000002)) {
          arr_ = java.util.Collections.unmodifiableList(arr_);
          bitField0_ = (bitField0_ & ~0x00000002);
        }
        result.arr_ = arr_;
        if (((from_bitField0_ & 0x00000004) == 0x00000004)) {
          to_bitField0_ |= 0x00000002;
        }
        result.mute_ = mute_;
        result.bitField0_ = to_bitField0_;
        onBuilt();
        return result;
      }
      
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof test.Test.Entry) {
          return mergeFrom((test.Test.Entry)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }
      
      public Builder mergeFrom(test.Test.Entry other) {
        if (other == test.Test.Entry.getDefaultInstance()) return this;
        if (other.hasData()) {
          setData(other.getData());
        }
        if (!other.arr_.isEmpty()) {
          if (arr_.isEmpty()) {
            arr_ = other.arr_;
            bitField0_ = (bitField0_ & ~0x00000002);
          } else {
            ensureArrIsMutable();
            arr_.addAll(other.arr_);
          }
          onChanged();
        }
        if (other.hasMute()) {
          setMute(other.getMute());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }
      
      public final boolean isInitialized() {
        return true;
      }
      
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        com.google.protobuf.UnknownFieldSet.Builder unknownFields =
          com.google.protobuf.UnknownFieldSet.newBuilder(
            this.getUnknownFields());
        while (true) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              this.setUnknownFields(unknownFields.build());
              onChanged();
              return this;
            default: {
              if (!parseUnknownField(input, unknownFields,
                                     extensionRegistry, tag)) {
                this.setUnknownFields(unknownFields.build());
                onChanged();
                return this;
              }
              break;
            }
            case 13: {
              bitField0_ |= 0x00000001;
              data_ = input.readFixed32();
              break;
            }
            case 21: {
              ensureArrIsMutable();
              arr_.add(input.readFixed32());
              break;
            }
            case 18: {
              int length = input.readRawVarint32();
              int limit = input.pushLimit(length);
              while (input.getBytesUntilLimit() > 0) {
                addArr(input.readFixed32());
              }
              input.popLimit(limit);
              break;
            }
            case 26: {
              bitField0_ |= 0x00000004;
              mute_ = input.readBytes();
              break;
            }
          }
        }
      }
      
      private int bitField0_;
      
      // optional fixed32 data = 1;
      private int data_ ;
      public boolean hasData() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      public int getData() {
        return data_;
      }
      public Builder setData(int value) {
        bitField0_ |= 0x00000001;
        data_ = value;
        onChanged();
        return this;
      }
      public Builder clearData() {
        bitField0_ = (bitField0_ & ~0x00000001);
        data_ = 0;
        onChanged();
        return this;
      }
      
      // repeated fixed32 arr = 2;
      private java.util.List<java.lang.Integer> arr_ = java.util.Collections.emptyList();;
      private void ensureArrIsMutable() {
        if (!((bitField0_ & 0x00000002) == 0x00000002)) {
          arr_ = new java.util.ArrayList<java.lang.Integer>(arr_);
          bitField0_ |= 0x00000002;
         }
      }
      public java.util.List<java.lang.Integer>
          getArrList() {
        return java.util.Collections.unmodifiableList(arr_);
      }
      public int getArrCount() {
        return arr_.size();
      }
      public int getArr(int index) {
        return arr_.get(index);
      }
      public Builder setArr(
          int index, int value) {
        ensureArrIsMutable();
        arr_.set(index, value);
        onChanged();
        return this;
      }
      public Builder addArr(int value) {
        ensureArrIsMutable();
        arr_.add(value);
        onChanged();
        return this;
      }
      public Builder addAllArr(
          java.lang.Iterable<? extends java.lang.Integer> values) {
        ensureArrIsMutable();
        super.addAll(values, arr_);
        onChanged();
        return this;
      }
      public Builder clearArr() {
        arr_ = java.util.Collections.emptyList();;
        bitField0_ = (bitField0_ & ~0x00000002);
        onChanged();
        return this;
      }
      
      // optional string mute = 3;
      private java.lang.Object mute_ = "";
      public boolean hasMute() {
        return ((bitField0_ & 0x00000004) == 0x00000004);
      }
      public String getMute() {
        java.lang.Object ref = mute_;
        if (!(ref instanceof String)) {
          String s = ((com.google.protobuf.ByteString) ref).toStringUtf8();
          mute_ = s;
          return s;
        } else {
          return (String) ref;
        }
      }
      public Builder setMute(String value) {
        if (value == null) {
    throw new NullPointerException();
  }
  bitField0_ |= 0x00000004;
        mute_ = value;
        onChanged();
        return this;
      }
      public Builder clearMute() {
        bitField0_ = (bitField0_ & ~0x00000004);
        mute_ = getDefaultInstance().getMute();
        onChanged();
        return this;
      }
      void setMute(com.google.protobuf.ByteString value) {
        bitField0_ |= 0x00000004;
        mute_ = value;
        onChanged();
      }
      
      // @@protoc_insertion_point(builder_scope:test.Entry)
    }
    
    static {
      defaultInstance = new Entry(true);
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:test.Entry)
  }
  
  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_test_Entry_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_test_Entry_fieldAccessorTable;
  
  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\ntest.proto\022\004test\"0\n\005Entry\022\014\n\004data\030\001 \001(" +
      "\007\022\013\n\003arr\030\002 \003(\007\022\014\n\004mute\030\003 \001(\t"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
      new com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner() {
        public com.google.protobuf.ExtensionRegistry assignDescriptors(
            com.google.protobuf.Descriptors.FileDescriptor root) {
          descriptor = root;
          internal_static_test_Entry_descriptor =
            getDescriptor().getMessageTypes().get(0);
          internal_static_test_Entry_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_test_Entry_descriptor,
              new java.lang.String[] { "Data", "Arr", "Mute", },
              test.Test.Entry.class,
              test.Test.Entry.Builder.class);
          return null;
        }
      };
    com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        }, assigner);
  }
  
  // @@protoc_insertion_point(outer_class_scope)
}
