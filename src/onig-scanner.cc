#include "onig-scanner.h"
#include "onig-scanner-worker.h"

using ::v8::FunctionTemplate;
using ::v8::Context;

void OnigScanner::Init(Local<Object> target) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(OnigScanner::New);
  tpl->SetClassName(Nan::New<String>("OnigScanner").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  Local<Context> context = Nan::GetCurrentContext();
  v8::Isolate* isolate = context->GetIsolate();
  tpl->PrototypeTemplate()->Set(isolate, "_findNextMatch", Nan::New<FunctionTemplate>(OnigScanner::FindNextMatch));
  tpl->PrototypeTemplate()->Set(isolate, "_findNextMatchSync", Nan::New<FunctionTemplate>(OnigScanner::FindNextMatchSync));

  target->Set(context, Nan::New<String>("OnigScanner").ToLocalChecked(), tpl->GetFunction(context).ToLocalChecked());
}

void InitModule(Local<Object> target) {
  OnigScanner::Init(target);
  OnigString::Init(target);
}

NODE_MODULE(onig_scanner, InitModule)

NAN_METHOD(OnigScanner::New) {
  Nan::HandleScope scope;
  OnigScanner* scanner = new OnigScanner(Local<Array>::Cast(info[0]));
  scanner->Wrap(info.This());
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(OnigScanner::FindNextMatchSync) {
  Nan::HandleScope scope;
  OnigScanner* scanner = node::ObjectWrap::Unwrap<OnigScanner>(info.This());

  Local<Object> param1 = Local<Object>::Cast(info[0]);
  Local<Number> param2 = Local<Number>::Cast(info[1]);
  Local<Value> result;

  if (param1->IsString()) {
    Local<String> v8String = Local<String>::Cast(info[0]);
    result = scanner->FindNextMatchSync(v8String, param2);
  } else {
    OnigString* onigString = node::ObjectWrap::Unwrap<OnigString>(info[0].As<Object>());
    result = scanner->FindNextMatchSync(onigString, param2);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(OnigScanner::FindNextMatch) {
  Nan::HandleScope scope;
  OnigScanner* scanner = node::ObjectWrap::Unwrap<OnigScanner>(info.This());
  scanner->FindNextMatch(Local<String>::Cast(info[0]), Local<Number>::Cast(info[1]), Local<Function>::Cast(info[2]));
  info.GetReturnValue().SetUndefined();
}

OnigScanner::OnigScanner(Local<Array> sources) {
  int length = sources->Length();
  regExps.resize(length);

  Local<Context> context = Nan::GetCurrentContext();
  for (int i = 0; i < length; i++) {
    Nan::Utf8String utf8Value(sources->Get(context, i).ToLocalChecked());
    regExps[i] = shared_ptr<OnigRegExp>(new OnigRegExp(string(*utf8Value)));
  }

  searcher = shared_ptr<OnigSearcher>(new OnigSearcher(regExps));
}

OnigScanner::~OnigScanner() {}

void OnigScanner::FindNextMatch(Local<String> v8String, Local<Number> v8StartLocation, Local<Function> v8Callback) {
  int charOffset = v8StartLocation->Value();
  Nan::Callback *callback = new Nan::Callback(v8Callback);

  OnigScannerWorker *worker = new OnigScannerWorker(callback, regExps, v8String, charOffset);
  Nan::AsyncQueueWorker(worker);
}

Local<Value> OnigScanner::FindNextMatchSync(Local<String> v8String, Local<Number> v8StartLocation) {
  OnigString* source = new OnigString(v8String);
  Local<Value> r = FindNextMatchSync(source, v8StartLocation);
  delete source;
  return r;
}

Local<Value> OnigScanner::FindNextMatchSync(OnigString* source, Local<Number> v8StartLocation) {
  int charOffset = v8StartLocation->Value();

  shared_ptr<OnigResult> bestResult = searcher->Search(source, charOffset);
  if (bestResult != NULL) {
    Local<Object> result = Nan::New<Object>();
    Local<Context> context = Nan::GetCurrentContext();
    result->Set(context, Nan::New<String>("index").ToLocalChecked(), Nan::New<Number>(bestResult->Index()));
    result->Set(context, Nan::New<String>("captureIndices").ToLocalChecked(), CaptureIndicesForMatch(bestResult.get(), source));
    return result;
  } else {
    return Nan::Null();
  }
}

Local<Value> OnigScanner::CaptureIndicesForMatch(OnigResult* result, OnigString* source) {
  int resultCount = result->Count();
  Local<Array> captures = Nan::New<Array>(resultCount);

  for (int index = 0; index < resultCount; index++) {
    int captureStart = source->ConvertUtf8OffsetToUtf16(result->LocationAt(index));
    int captureEnd = source->ConvertUtf8OffsetToUtf16(result->LocationAt(index) + result->LengthAt(index));

    Local<Object> capture = Nan::New<Object>();
    Local<Context> context = Nan::GetCurrentContext();
    capture->Set(context, Nan::New<String>("index").ToLocalChecked(), Nan::New<Number>(index));
    capture->Set(context, Nan::New<String>("start").ToLocalChecked(), Nan::New<Number>(captureStart));
    capture->Set(context, Nan::New<String>("end").ToLocalChecked(), Nan::New<Number>(captureEnd));
    capture->Set(context, Nan::New<String>("length").ToLocalChecked(), Nan::New<Number>(captureEnd - captureStart));
    captures->Set(context, index, capture);
  }

  return captures;
}
