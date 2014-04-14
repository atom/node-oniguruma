#include "onig-cache.h"
#include "unicode-utils.h"

void OnigCache::Clear() {
  maxCachedIndex = -1;
  results.clear();
}

void OnigCache::Init(const string &stringToSearch, int byteOffset) {
  useCache = (stringToSearch == lastMatchedString && byteOffset >= lastStartLocation);
  lastStartLocation = byteOffset;

  if (!useCache) {
    Clear();
    lastMatchedString = stringToSearch;
  }
}

shared_ptr<OnigResult> OnigCache::Search(OnigRegExp *regExp, const string &searchString, int byteOffset, bool hasMultibyteCharacters) {
  shared_ptr<OnigResult> result;
  int index = regExp->Index();
  bool useCachedResult = false;

  if (useCache && index <= maxCachedIndex) {
    result = results[index];
    if (result != NULL) {
      int location = result->LocationAt(0);
      if (hasMultibyteCharacters) {
        location = UnicodeUtils::characters_in_bytes(searchString.data(), location);
      }
      useCachedResult = location >= byteOffset;
    } else {
      useCachedResult = true;
    }
  }

  if (!useCachedResult) {
    result = regExp->Search(searchString, byteOffset);
    results[index] = result;
    maxCachedIndex = index;
  }

  return result;
}
