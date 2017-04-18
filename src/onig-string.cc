#include "onig-string.h"

using namespace v8;

void OnigString::Init(Local<Object> target) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(OnigString::New);
  tpl->SetClassName(Nan::New<String>("OnigString").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("length").ToLocalChecked(), Length);
  target->Set(Nan::New<String>("OnigString").ToLocalChecked(), tpl->GetFunction());
}

void OnigString::New(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  Nan::HandleScope scope;
  OnigString* scanner = new OnigString(Local<String>::Cast(info[0]));
  scanner->Wrap(info.This());
  info.GetReturnValue().SetUndefined();
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
