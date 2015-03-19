#ifndef SRC_ONIG_STRING_CONTEXT_H_
#define SRC_ONIG_STRING_CONTEXT_H_

#include <string>
#include "nan.h"

using ::v8::CopyablePersistentTraits;
using ::v8::Handle;
using ::v8::Isolate;
using ::v8::Persistent;
using ::v8::String;
using ::v8::Value;

using ::std::string;

class OnigStringContext {
 public:
  explicit OnigStringContext(Handle<String> v8String);
  bool IsSame(Handle<String> other) const;
  bool HasMultibyteCharacters() const;
  const char* utf8_value() const { return *utf8Value; }
  size_t utf8_length() const { return utf8Value.length(); }
  const wchar_t* utf16_value() const { return reinterpret_cast<const wchar_t*>(*utf16Value); }
  size_t utf16_length() const { return utf16Value.length(); }
  bool has_multibyte_characters() const { return hasMultibyteCharacters; }

 private:
  typedef Persistent<String, CopyablePersistentTraits<String> > CopyableStringPersistent;

  CopyableStringPersistent v8String;
  String::Utf8Value utf8Value;
  String::Value utf16Value;
  bool hasMultibyteCharacters;
};
#endif  // SRC_ONIG_STRING_CONTEXT_H_
