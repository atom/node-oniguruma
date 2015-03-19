#include "onig-string-context.h"


OnigStringContext::OnigStringContext(Handle<String> str)
  : v8String(Isolate::GetCurrent(), str),
    utf8Value(str),
#ifdef _WIN32
    utf16Value(str),
#else
    utf16Value(Handle<String>()),
#endif
    hasMultibyteCharacters(str->Length() != str->Utf8Length()) {
}

bool OnigStringContext::IsSame(Handle<String> other) const {
  return v8String == other;
}
