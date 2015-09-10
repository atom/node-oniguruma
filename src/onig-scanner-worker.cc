#include "onig-scanner-worker.h"
#include "onig-string-context.h"
#include "unicode-utils.h"

using ::v8::Array;
using ::v8::Local;
using ::v8::Null;
using ::v8::Number;
using ::v8::Object;
using ::v8::String;
using ::v8::Value;

void OnigScannerWorker::Execute() {
  bestResult = searcher->Search(source, charOffset);
}

void OnigScannerWorker::HandleOKCallback() {
  Nan::HandleScope scope;

  // Try to reuse the cached results across async searches
  cache->Reset(searcher->GetCache());

  if (bestResult != NULL) {
    Local<Object> result = Nan::New<Object>();
    result->Set(Nan::New<String>("index").ToLocalChecked(), Nan::New<Number>(bestResult->Index()));

    int resultCount = bestResult->Count();
    Local<Array> captures = Nan::New<Array>(resultCount);
    for (int index = 0; index < resultCount; index++) {
      int captureLength = bestResult->LengthAt(index);
      int captureStart = bestResult->LocationAt(index);

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
    result->Set(Nan::New<String>("captureIndices").ToLocalChecked(), captures);

    Local<Value> argv[] = {
      Nan::Null(),
      result
    };
    callback->Call(2, argv);
  } else {
    Local<Value> argv[] = {
      Nan::Null(),
      Nan::Null()
    };
    callback->Call(2, argv);
  }
}
