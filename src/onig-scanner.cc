#include "onig-scanner.h"
#include "onig-reg-exp.h"
#include "onig-result.h"
#include "unicode-utils.h"

using ::v8::Function;
using ::v8::FunctionTemplate;
using ::v8::HandleScope;
using ::v8::Local;
using ::v8::Null;

void OnigScanner::Init(Handle<Object> target) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(OnigScanner::New);
  tpl->SetClassName(String::NewSymbol("OnigScanner"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->PrototypeTemplate()->Set(String::NewSymbol("findNextMatch"), FunctionTemplate::New(OnigScanner::FindNextMatch)->GetFunction());

  target->Set(String::NewSymbol("OnigScanner"), tpl->GetFunction());
}

NODE_MODULE(onig_scanner, OnigScanner::Init)

NAN_METHOD(OnigScanner::New) {
  NanScope();
  OnigScanner* scanner = new OnigScanner(Local<Array>::Cast(args[0]));
  scanner->Wrap(args.This());
  NanReturnUndefined();
}

NAN_METHOD(OnigScanner::FindNextMatch) {
  NanScope();
  OnigScanner* scanner = node::ObjectWrap::Unwrap<OnigScanner>(args.This());
  NanReturnValue(scanner->FindNextMatch(Local<String>::Cast(args[0]), Local<Number>::Cast(args[1]), args.This()));
}

OnigScanner::OnigScanner(Handle<Array> sources) {
  int length = sources->Length();
  regExps.resize(length);
  cachedResults.resize(length);

  for (int i = 0; i < length; i++) {
    String::Utf8Value utf8Value(sources->Get(i));
    regExps[i] = unique_ptr<OnigRegExp>(new OnigRegExp(string(*utf8Value)));
  }
}

OnigScanner::~OnigScanner() {}

Handle<Value> OnigScanner::FindNextMatch(Handle<String> v8String, Handle<Number> v8StartLocation, Handle<Value> v8Scanner) {
  String::Utf8Value utf8Value(v8String);
  string string(*utf8Value);
  int charOffset = v8StartLocation->Value();

#ifdef _WIN32
  String::Value utf16Value(v8String);
  int byteOffset = UnicodeUtils::bytes_in_characters(
      reinterpret_cast<const wchar_t*>(*utf16Value), charOffset);
#else
  int byteOffset = UnicodeUtils::bytes_in_characters(*utf8Value, charOffset);
#endif
  int bestIndex = -1;
  int bestLocation = 0;
  OnigResult* bestResult = NULL;

  bool useCachedResults = (string == lastMatchedString && byteOffset >= lastStartLocation);
  lastStartLocation = byteOffset;

  if (!useCachedResults) {
    ClearCachedResults();
    lastMatchedString = string;
  }

  vector< unique_ptr<OnigRegExp> >::iterator iter = regExps.begin();
  int index = 0;
  while (iter < regExps.end()) {
    OnigRegExp *regExp = (*iter).get();

    bool useCachedResult = false;
    OnigResult *result = NULL;

    if (useCachedResults && index <= maxCachedIndex) {
      result = cachedResults[index].get();
      useCachedResult = (result == NULL || result->LocationAt(0) >= charOffset);
    }

    if (!useCachedResult) {
      result = regExp->Search(string, byteOffset);
      cachedResults[index] = unique_ptr<OnigResult>(result);
      maxCachedIndex = index;
    }

    if (result && result->Count() > 0) {
      int location = result->LocationAt(0);
      if (bestIndex == -1 || location < bestLocation) {
        bestLocation = location;
        bestResult = result;
        bestIndex = index;
      }

      if (location == charOffset) {
        break;
      }
    }

    ++iter;
    index++;
  }

  if (bestIndex >= 0) {
    Local<Object> result = Object::New();
    result->Set(String::NewSymbol("index"), Number::New(bestIndex));
    result->Set(String::NewSymbol("captureIndices"), CaptureIndicesForMatch(bestResult));
    result->Set(String::NewSymbol("scanner"), v8Scanner);
    return result;
  } else {
    return Null();
  }
}

void OnigScanner::ClearCachedResults() {
  maxCachedIndex = -1;
  cachedResults.clear();
}

Handle<Value> OnigScanner::CaptureIndicesForMatch(OnigResult* result) {
  int resultCount = result->Count();
  Local<Array> captures = Array::New(resultCount);
  for (int index = 0; index < resultCount; index++) {
    int captureLength = result->LengthAt(index);
    int captureStart = result->LocationAt(index);

    Local<Object> capture = Object::New();
    capture->Set(String::NewSymbol("index"), Number::New(index));
    capture->Set(String::NewSymbol("start"), Number::New(captureStart));
    capture->Set(String::NewSymbol("end"), Number::New(captureStart + captureLength));
    capture->Set(String::NewSymbol("length"), Number::New(captureLength));
    captures->Set(index, capture);
  }

  return captures;
}
