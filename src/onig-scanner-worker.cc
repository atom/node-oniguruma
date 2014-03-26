#include "onig-scanner-worker.h"

using ::v8::Array;
using ::v8::Local;
using ::v8::Null;
using ::v8::Number;
using ::v8::Object;
using ::v8::String;
using ::v8::Value;

void OnigScannerWorker::Execute() {
  int byteOffset = charOffset;
  if (hasMultibyteCharacters) {
#ifdef _WIN32
    byteOffset = UnicodeUtils::bytes_in_characters(utf16StringToSearch.get(), charOffset);
#else
    byteOffset = UnicodeUtils::bytes_in_characters(stringToSearch.data(), charOffset);
#endif
  }

  int bestLocation = 0;
  vector< shared_ptr<OnigRegExp> >::iterator iter = regExps.begin();
  int index = 0;
  while (iter < regExps.end()) {
    OnigRegExp *regExp = (*iter).get();

    bool useCachedResult = false;
    shared_ptr<OnigResult> result;

    if (useCachedResults && index <= maxCachedIndex) {
      result = cachedResults[index];
      if (result) {
        int location = result->LocationAt(0);
        if (hasMultibyteCharacters) {
          location = UnicodeUtils::characters_in_bytes(stringToSearch.data(), location);
        }
        useCachedResult = location >= charOffset;
      } else {
        useCachedResult = true;
      }
    }

    if (!useCachedResult) {
      result = regExp->Search(stringToSearch, byteOffset);
      // TODO(kevinsawicki): add to cache
      maxCachedIndex = index;
    }

    if (result != NULL && result->Count() > 0) {
      int location = result->LocationAt(0);
      if (hasMultibyteCharacters) {
        location =  UnicodeUtils::characters_in_bytes(stringToSearch.data(), location);
      }

      if (bestResult.get() == NULL || location < bestLocation) {
        bestLocation = location;
        bestResult = result;
      }

      if (location == charOffset) {
        break;
      }
    }

    ++iter;
    index++;
  }
}

void OnigScannerWorker::HandleOKCallback() {
  NanScope();

  if (bestResult.get() != NULL) {
    Local<Object> result = Object::New();
    result->Set(String::NewSymbol("index"), Number::New(bestResult.get()->Index()));

    int resultCount = bestResult.get()->Count();
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