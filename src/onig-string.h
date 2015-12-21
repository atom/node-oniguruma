#ifndef SRC_ONIG_STRING_H_
#define SRC_ONIG_STRING_H_

#include <memory>

#include "nan.h"

using ::std::shared_ptr;

using ::v8::Local;
using ::v8::Object;
using ::v8::String;

class OnigString : public node::ObjectWrap {
 public:
  static void Init(Local<Object> target);
  explicit OnigString(Local<String> value);
  ~OnigString();

  int uniqueId() { return uniqueId_; }

  const char* utf8_value() const { return *utf8Value; }
  size_t utf8_length() const { return utf8Value.length(); }

  int ConvertUtf8OffsetToUtf16(int utf8Offset);
  int ConvertUtf16OffsetToUtf8(int utf16Offset);
  int ConvertUnicodeLengthToUtf16(int utf16Offset, int codePointLength);

 private:
  static NAN_METHOD(New);

  int uniqueId_;
  String::Utf8Value utf8Value;
  bool hasMultiByteChars;
  int *utf16OffsetToUtf8;
  int *utf8OffsetToUtf16;
  bool *utf16OffsetIsCodePointEnd;
};

#endif  // SRC_ONIG_STRING_H_
