#ifndef SRC_ONIG_STRING_H_
#define SRC_ONIG_STRING_H_

#include <memory>
#include "nan.h"

class OnigString : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> target);
  explicit OnigString(v8::Local<v8::String> value);

  int uniqueId;
  std::vector<uint16_t> content;

 private:
  static void New(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void ToString(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void Substring(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void Length(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &);
};

#endif  // SRC_ONIG_STRING_H_
