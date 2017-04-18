#include "onig-string.h"

using namespace v8;

void OnigString::Init(Local<Object> target) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(OnigString::New);
  tpl->SetClassName(Nan::New<String>("OnigString").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->PrototypeTemplate()->Set(Nan::New<String>("toString").ToLocalChecked(), Nan::New<FunctionTemplate>(ToString));
  tpl->PrototypeTemplate()->Set(Nan::New<String>("substring").ToLocalChecked(), Nan::New<FunctionTemplate>(Substring));
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("length").ToLocalChecked(), Length);
  target->Set(Nan::New<String>("OnigString").ToLocalChecked(), tpl->GetFunction());
}

void OnigString::New(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info[0]->IsString()) {
    OnigString* scanner = new OnigString(Local<String>::Cast(info[0]));
    scanner->Wrap(info.This());
  } else {
    Nan::ThrowTypeError("Argument must be a string");
  }
}

void OnigString::ToString(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  OnigString *string = node::ObjectWrap::Unwrap<OnigString>(info.This());
  Local<String> result;
  if (Nan::New<String>(string->content.data(), string->content.size()).ToLocal(&result)) {
    info.GetReturnValue().Set(result);
  }
}

static int32_t constrain(int32_t min, int32_t value, int32_t max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

void OnigString::Substring(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  OnigString *string = node::ObjectWrap::Unwrap<OnigString>(info.This());
  Local<String> result;

  const uint16_t *data = string->content.data();
  const uint32_t length = string->content.size();

  int32_t start_index = 0;
  int32_t end_index = length;

  if (info[0]->IsNumber()) {
    start_index = constrain(0, info[0]->IntegerValue(), length);

    if (info[1]->IsNumber()) {
      end_index = constrain(0, info[1]->IntegerValue(), length);
    }
  }

  if (Nan::New<String>(data + start_index, end_index - start_index).ToLocal(&result)) {
    info.GetReturnValue().Set(result);
  }
}

void OnigString::Length(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  OnigString *string = node::ObjectWrap::Unwrap<OnigString>(info.This());
  info.GetReturnValue().Set(Nan::New<Number>(string->content.size()));
}

OnigString::OnigString(Local<String> value) {
  static int nextId = 0;
  uniqueId = ++nextId;
  content.resize(value->Length());
  value->Write(content.data(), 0, -1, String::NO_NULL_TERMINATION);
}
