#include "onig-string-context.h"
#include <node.h>  // To see NODE_MINOR_VERSION

OnigStringContext::OnigStringContext(Local<String> str)
// The v8::Persistent API changed recently and now it requires v8::Isolate
// to the constructor argument.
  : v8String(str),
    utf8Value(str),
#ifdef _WIN32
    utf16Value(str),
#endif
    hasMultibyteCharacters(str->Length() != str->Utf8Length()) {
}

bool OnigStringContext::IsSame(Local<String> other) {
  return v8String == other;
}
