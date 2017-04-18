#include "onig-string.h"

using namespace v8;

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

OnigString::OnigString(Local<String> value) {
  static int nextId = 0;
  uniqueId = ++nextId;
  content.resize(value->Length());
  value->Write(content.data(), 0, -1, String::NO_NULL_TERMINATION);
}
