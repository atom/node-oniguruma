#include "onig-string.h"

using ::v8::FunctionTemplate;

void OnigString::Init(Local<Object> target) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(OnigString::New);
  tpl->SetClassName(Nan::New<String>("OnigString").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  target->Set(Nan::New<String>("OnigString").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(OnigString::New) {
  Nan::HandleScope scope;
  OnigString* scanner = new OnigString(Local<String>::Cast(info[0]));
  scanner->Wrap(info.This());
  info.GetReturnValue().SetUndefined();
}

OnigString::OnigString(Local<String> value)
  : utf8Value(value), utf8_length_(utf8Value.length()) {
  static int idGenerator = 0;
  uniqueId_ = ++idGenerator;

  hasMultiByteChars = (value->Length() != utf8_length_);

  if (hasMultiByteChars) {
    String::Value utf16Value(value);
    utf16_length_ = utf16Value.length();

    utf16OffsetToUtf8 = new int[utf16_length_ + 1];
    utf16OffsetToUtf8[utf16_length_] = utf8_length_;

    utf8OffsetToUtf16 = new int[utf8_length_ + 1];
    utf8OffsetToUtf16[utf8_length_] = utf16_length_;

    // http://stackoverflow.com/a/148766
    unsigned int codepoint = 0;
    int i16_codepoint_start = 0;
    int i8 = 0;
    for (int i16 = 0, len = utf16_length_; i16 < len; i16++) {
      uint16_t in = (*utf16Value)[i16];

      utf16OffsetToUtf8[i16] = i8;

      if (in >= 0xd800 && in <= 0xdbff) {
        codepoint = ((in - 0xd800) << 10) + 0x10000;
      } else {
        if (in >= 0xdc00 && in <= 0xdfff) {
          codepoint |= in - 0xdc00;
        } else {
          codepoint = in;
        }

        if (codepoint <= 0x7f) {
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
        } else if (codepoint <= 0x7ff) {
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
        } else if (codepoint <= 0xffff) {
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
        } else {
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
          utf8OffsetToUtf16[i8] = i16_codepoint_start;
          i8++;
        }
        codepoint = 0;
        i16_codepoint_start = i16 + 1;
      }
    }
  }
}

OnigString::~OnigString() {
  if (hasMultiByteChars) {
    delete []utf16OffsetToUtf8;
    delete []utf8OffsetToUtf16;
  }
}

int OnigString::ConvertUtf8OffsetToUtf16(int utf8Offset) {
  if (hasMultiByteChars) {
    return utf8OffsetToUtf16[utf8Offset];
  }
  return utf8Offset;
}

int OnigString::ConvertUtf16OffsetToUtf8(int utf16Offset) {
  if (hasMultiByteChars) {
    return utf16OffsetToUtf8[utf16Offset];
  }
  return utf16Offset;
}
