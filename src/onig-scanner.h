#include <node.h>
#include <v8.h>
#include <string>
#include <vector>
#include <memory>

using namespace v8;
using namespace std;

class OnigRegExp;
class OnigResult;

class OnigScanner : public node::ObjectWrap {
  public:
    static void Init(Handle<Object> target);

  private:
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> FindNextMatch(const Arguments& args);
    OnigScanner(Handle<Array> sources);
    ~OnigScanner();

    Handle<Value> FindNextMatch(Handle<String> v8String, Handle<Number> v8StartLocation, Handle<Value> v8Scanner);
    Handle<Value> CaptureIndicesForMatch(OnigResult* result);
    void ClearCachedResults();

    vector<unique_ptr<OnigRegExp>> regExps;
    vector<unique_ptr<OnigResult>> cachedResults;
    string lastMatchedString;
    int maxCachedIndex;
    int lastStartLocation;
};
