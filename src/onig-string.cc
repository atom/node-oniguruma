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
  : utf8Value(value) {
  static int idGenerator = 0;
  uniqueId_ = ++idGenerator;

  hasMultiByteChars = (value->Length() != value->Utf8Length());

  if (hasMultiByteChars) {
    String::Value utf16Value(value);

    utf16OffsetToUtf8 = new int[utf16Value.length()];
    utf16OffsetIsCodePointEnd = new bool[utf16Value.length()];
    utf8OffsetToUtf16 = new int[utf8Value.length()];

    // http://stackoverflow.com/a/148766
    unsigned int codepoint = 0;
    int i8 = 0;
    for (int i16 = 0, len = utf16Value.length(); i16 < len; i16++) {
      uint16_t in = (*utf16Value)[i16];

      utf16OffsetToUtf8[i16] = i8;
      utf16OffsetIsCodePointEnd[i16] = false;

      if (in >= 0xd800 && in <= 0xdbff) {
        codepoint = ((in - 0xd800) << 10) + 0x10000;
      } else {
        if (in >= 0xdc00 && in <= 0xdfff) {
          codepoint |= in - 0xdc00;
        } else {
          codepoint = in;
        }

        utf16OffsetIsCodePointEnd[i16] = true;
        if (codepoint <= 0x7f) {
          utf8OffsetToUtf16[i8] = i16;
          i8++;
        } else if (codepoint <= 0x7ff) {
          utf8OffsetToUtf16[i8] = i16;
          i8++;
          utf8OffsetToUtf16[i8] = i16;
          i8++;
        } else if (codepoint <= 0xffff) {
          utf8OffsetToUtf16[i8] = i16 - 1;
          i8++;
          utf8OffsetToUtf16[i8] = i16 - 1;
          i8++;
          utf8OffsetToUtf16[i8] = i16 - 1;
          i8++;
        } else {
          utf8OffsetToUtf16[i8] = i16 - 1;
          i8++;
          utf8OffsetToUtf16[i8] = i16 - 1;
          i8++;
          utf8OffsetToUtf16[i8] = i16 - 1;
          i8++;
          utf8OffsetToUtf16[i8] = i16 - 1;
          i8++;
        }
        codepoint = 0;
      }
    }
  }
}

OnigString::~OnigString() {
  if (hasMultiByteChars) {
    delete []utf16OffsetToUtf8;
    delete []utf16OffsetIsCodePointEnd;
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

int OnigString::ConvertUnicodeLengthToUtf16(int utf16Offset, int codePointLength) {
  if (hasMultiByteChars) {
    int result = 0;
    while (codePointLength > 0) {
      bool isCodePointEnd = utf16OffsetIsCodePointEnd[utf16Offset + result];
      if (isCodePointEnd) {
        codePointLength--;
      }
      result++;
    }
    return result;
  }

  return codePointLength;
}
