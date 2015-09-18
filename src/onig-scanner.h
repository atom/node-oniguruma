#ifndef SRC_ONIG_SCANNER_H_
#define SRC_ONIG_SCANNER_H_

#include <string>
#include <vector>
#include <memory>

#include "nan.h"
#include "onig-cache.h"
#include "onig-searcher.h"

using ::v8::Array;
using ::v8::Function;
using ::v8::Local;
using ::v8::Number;
using ::v8::Object;
using ::v8::String;
using ::v8::Value;

using ::std::string;
using ::std::shared_ptr;
using ::std::vector;

class OnigRegExp;
class OnigResult;
class OnigStringContext;

class OnigScanner : public node::ObjectWrap {
 public:
  static void Init(Local<Object> target);

 private:
  static NAN_METHOD(New);
  static NAN_METHOD(FindNextMatch);
  static NAN_METHOD(FindNextMatchSync);
  explicit OnigScanner(Local<Array> sources);
  ~OnigScanner();

  void FindNextMatch(Local<String> v8String, Local<Number> v8StartLocation, Local<Function> v8Callback);
  Local<Value> FindNextMatchSync(Local<String> v8String, Local<Number> v8StartLocation);
  Local<Value> CaptureIndicesForMatch(OnigResult* result, shared_ptr<OnigStringContext> source);

  vector<shared_ptr<OnigRegExp>> regExps;
  shared_ptr<OnigSearcher> searcher;
  shared_ptr<OnigCache> asyncCache;
  shared_ptr<OnigStringContext> lastSource;
};

#endif  // SRC_ONIG_SCANNER_H_
