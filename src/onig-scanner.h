#ifndef SRC_ONIG_SCANNER_H_
#define SRC_ONIG_SCANNER_H_

#include <string>
#include <vector>
#include <memory>

#include "nan.h"

using ::v8::Array;
using ::v8::Handle;
using ::v8::Number;
using ::v8::Object;
using ::v8::String;
using ::v8::Value;

using ::std::string;
using ::std::shared_ptr;
using ::std::vector;

class OnigRegExp;
class OnigResult;

class OnigScanner : public node::ObjectWrap {
  public:
    static void Init(Handle<Object> target);

  private:
    static NAN_METHOD(New);
    static NAN_METHOD(FindNextMatch);
    explicit OnigScanner(Handle<Array> sources);
    ~OnigScanner();

    Handle<Value> FindNextMatch(Handle<String> v8String, Handle<Number> v8StartLocation, Handle<Value> v8Scanner);
    Handle<Value> CaptureIndicesForMatch(OnigResult* result, Handle<String> v8String, const char* string, bool hasMultibyteCharacters);
    void ClearCachedResults();

    vector<shared_ptr<OnigRegExp>> regExps;
    vector<shared_ptr<OnigResult>> cachedResults;
    string lastMatchedString;
    int maxCachedIndex;
    int lastStartLocation;
};

#endif  // SRC_ONIG_SCANNER_H_
