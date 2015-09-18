#include "onig-scanner.h"
#include "onig-string-context.h"
#include "onig-reg-exp.h"
#include "onig-result.h"
#include "onig-scanner-worker.h"
#include "unicode-utils.h"

using ::v8::Function;
using ::v8::FunctionTemplate;
using ::v8::HandleScope;
using ::v8::Local;
using ::v8::Null;
using ::v8::String;

void OnigScanner::Init(Local<Object> target) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(OnigScanner::New);
  tpl->SetClassName(Nan::New<String>("OnigScanner").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->PrototypeTemplate()->Set(Nan::New<String>("_findNextMatch").ToLocalChecked(), Nan::New<FunctionTemplate>(OnigScanner::FindNextMatch)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<String>("_findNextMatchSync").ToLocalChecked(), Nan::New<FunctionTemplate>(OnigScanner::FindNextMatchSync)->GetFunction());

  target->Set(Nan::New<String>("OnigScanner").ToLocalChecked(), tpl->GetFunction());
}

NODE_MODULE(onig_scanner, OnigScanner::Init)

NAN_METHOD(OnigScanner::New) {
  Nan::HandleScope scope;
  OnigScanner* scanner = new OnigScanner(Local<Array>::Cast(info[0]));
  scanner->Wrap(info.This());
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(OnigScanner::FindNextMatchSync) {
  Nan::HandleScope scope;
  OnigScanner* scanner = node::ObjectWrap::Unwrap<OnigScanner>(info.This());
  info.GetReturnValue().Set((scanner->FindNextMatchSync(Local<String>::Cast(info[0]), Local<Number>::Cast(info[1]))));
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

  for (int i = 0; i < length; i++) {
    String::Utf8Value utf8Value(sources->Get(i));
    regExps[i] = shared_ptr<OnigRegExp>(new OnigRegExp(string(*utf8Value), i));
  }

  searcher = shared_ptr<OnigSearcher>(new OnigSearcher(regExps));
  asyncCache = shared_ptr<OnigCache>(new OnigCache(length));
}

OnigScanner::~OnigScanner() {}

void OnigScanner::FindNextMatch(Local<String> v8String, Local<Number> v8StartLocation, Local<Function> v8Callback) {
  int charOffset = v8StartLocation->Value();
  Nan::Callback *callback = new Nan::Callback(v8Callback);
  shared_ptr<OnigStringContext> source = shared_ptr<OnigStringContext>(new OnigStringContext(v8String));


  OnigScannerWorker *worker = new OnigScannerWorker(callback, regExps, source, charOffset, asyncCache);
  Nan::AsyncQueueWorker(worker);
}

Local<Value> OnigScanner::FindNextMatchSync(Local<String> v8String, Local<Number> v8StartLocation) {
  if (!lastSource || !lastSource->IsSame(v8String))
    lastSource = shared_ptr<OnigStringContext>(new OnigStringContext(v8String));
  int charOffset = v8StartLocation->Value();

  shared_ptr<OnigResult> bestResult = searcher->Search(lastSource, charOffset);
  if (bestResult != NULL) {
    Local<Object> result = Nan::New<Object>();
    result->Set(Nan::New<String>("index").ToLocalChecked(), Nan::New<Number>(bestResult->Index()));
    result->Set(Nan::New<String>("captureIndices").ToLocalChecked(), CaptureIndicesForMatch(bestResult.get(), lastSource));
    return result;
  } else {
    return Nan::Null();
  }
}

Local<Value> OnigScanner::CaptureIndicesForMatch(OnigResult* result, shared_ptr<OnigStringContext> source) {
  int resultCount = result->Count();
  Local<Array> captures = Nan::New<Array>(resultCount);

  for (int index = 0; index < resultCount; index++) {
    int captureLength = result->LengthAt(index);
    int captureStart = result->LocationAt(index);

    if (source->has_multibyte_characters()) {
      captureLength = UnicodeUtils::characters_in_bytes(source->utf8_value() + captureStart, captureLength);
      captureStart = UnicodeUtils::characters_in_bytes(source->utf8_value(), captureStart);
    }

    Local<Object> capture = Nan::New<Object>();
    capture->Set(Nan::New<String>("index").ToLocalChecked(), Nan::New<Number>(index));
    capture->Set(Nan::New<String>("start").ToLocalChecked(), Nan::New<Number>(captureStart));
    capture->Set(Nan::New<String>("end").ToLocalChecked(), Nan::New<Number>(captureStart + captureLength));
    capture->Set(Nan::New<String>("length").ToLocalChecked(), Nan::New<Number>(captureLength));
    captures->Set(index, capture);
  }

  return captures;
}
