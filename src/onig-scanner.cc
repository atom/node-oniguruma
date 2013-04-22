#include "oniguruma.h"
#include "onig-scanner.h"
#include "onig-reg-exp.h"
#include "onig-result.h"
#include "unicode-utils.h"

void OnigScanner::Init(Handle<Object> target) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(OnigScanner::New);
  tpl->SetClassName(String::NewSymbol("OnigScanner"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->PrototypeTemplate()->Set(String::NewSymbol("findNextMatch"), FunctionTemplate::New(OnigScanner::FindNextMatch)->GetFunction());

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("OnigScanner"), constructor);
}

NODE_MODULE(onig_scanner, OnigScanner::Init)

Handle<Value> OnigScanner::New(const Arguments& args) {
  HandleScope scope;
  OnigScanner* scanner = new OnigScanner(Local<Array>::Cast(args[0]));
  scanner->Wrap(args.This());
  return args.This();
}

Handle<Value> OnigScanner::FindNextMatch(const Arguments& args) {
  HandleScope scope;
  OnigScanner* scanner = node::ObjectWrap::Unwrap<OnigScanner>(args.This());
  return scope.Close(scanner->FindNextMatch(Local<String>::Cast(args[0]), Local<Number>::Cast(args[1]), args.This()));
}

OnigScanner::OnigScanner(Handle<Array> sources) {
  int length = sources->Length();
  regExps.resize(length);
  cachedResults.resize(length);

  for (int i = 0; i < length; i++) {
    String::Utf8Value utf8Value(sources->Get(i));
    regExps[i] = unique_ptr<OnigRegExp>(new OnigRegExp(string(*utf8Value)));
  }
};

OnigScanner::~OnigScanner() {};

Handle<Value> OnigScanner::FindNextMatch(Handle<String> v8String, Handle<Number> v8StartLocation, Handle<Value> v8Scanner) {
  String::Utf8Value utf8Value(v8String);
  string string(*utf8Value);
  int startLocation = UnicodeUtils::bytes_in_characters(string.data(), v8StartLocation->Value());
  int bestIndex = -1;
  int bestLocation = NULL;
  OnigResult* bestResult = NULL;

  bool useCachedResults = (string == lastMatchedString && startLocation >= lastStartLocation);
  lastStartLocation = startLocation;

  if (!useCachedResults) {
    ClearCachedResults();
    lastMatchedString = string;
  }

  vector<unique_ptr<OnigRegExp>>::iterator iter = regExps.begin();
  int index = 0;
  while (iter < regExps.end()) {
    OnigRegExp *regExp = (*iter).get();

    bool useCachedResult = false;
    OnigResult *result = nullptr;

    if (useCachedResults && index <= maxCachedIndex) {
      result = cachedResults[index].get();
      useCachedResult = (result == NULL || result->LocationAt(0) >= startLocation);
    }

    if (!useCachedResult) {
      result = regExp->Search(string, startLocation);
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

      if (location == startLocation) {
        break;
      }
    }

    iter++;
    index++;
  }

  if (bestIndex >= 0) {
    Local<Object> result = Object::New();
    result->Set(String::NewSymbol("index"), Number::New(bestIndex));
    result->Set(String::NewSymbol("captureIndices"), CaptureIndicesForMatch(bestResult));
    result->Set(String::NewSymbol("scanner"), v8Scanner);
    return result;
  }
  else {
    return Null();
  }
}

void OnigScanner::ClearCachedResults() {
  maxCachedIndex = -1;
  cachedResults.clear();
}

Handle<Value> OnigScanner::CaptureIndicesForMatch(OnigResult* result) {
  int resultCount = result->Count();
  Local<Array> array = Array::New(resultCount * 3);
  int i = 0;
  for (int index = 0; index < resultCount; index++) {
    int captureLength = result->LengthAt(index);
    int captureStart = result->LocationAt(index);

    array->Set(i++, Number::New(index));
    array->Set(i++, Number::New(captureStart));
    array->Set(i++, Number::New(captureStart + captureLength));
  }

  return array;
}
