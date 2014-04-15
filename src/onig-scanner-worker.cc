#include "onig-scanner-worker.h"
#include "unicode-utils.h"

using ::v8::Array;
using ::v8::Local;
using ::v8::Null;
using ::v8::Number;
using ::v8::Object;
using ::v8::String;
using ::v8::Value;

void OnigScannerWorker::Execute() {
  bestResult = searcher->Search(stringToSearch, utf16StringToSearch.get(), hasMultibyteCharacters, charOffset);
}

void OnigScannerWorker::HandleOKCallback() {
  NanScope();

  // Try to reuse the cached results across async searches
  cache->Reset(searcher->GetCache());

  if (bestResult != NULL) {
    Local<Object> result = Object::New();
    result->Set(String::NewSymbol("index"), Number::New(bestResult->Index()));

    int resultCount = bestResult->Count();
    Local<Array> captures = Array::New(resultCount);
    for (int index = 0; index < resultCount; index++) {
      int captureLength = bestResult->LengthAt(index);
      int captureStart = bestResult->LocationAt(index);

      if (hasMultibyteCharacters) {
        captureLength = UnicodeUtils::characters_in_bytes(stringToSearch.data() + captureStart, captureLength);
        captureStart = UnicodeUtils::characters_in_bytes(stringToSearch.data(), captureStart);
      }

      Local<Object> capture = Object::New();
      capture->Set(String::NewSymbol("index"), Number::New(index));
      capture->Set(String::NewSymbol("start"), Number::New(captureStart));
      capture->Set(String::NewSymbol("end"), Number::New(captureStart + captureLength));
      capture->Set(String::NewSymbol("length"), Number::New(captureLength));
      captures->Set(index, capture);
    }
    result->Set(String::NewSymbol("captureIndices"), captures);

    Local<Value> argv[] = {
      Local<Value>::New(Null()),
      Local<Value>::New(result)
    };
    callback->Call(2, argv);
  } else {
    Local<Value> argv[] = {
      Local<Value>::New(Null()),
      Local<Value>::New(Null())
    };
    callback->Call(2, argv);
  }
}
