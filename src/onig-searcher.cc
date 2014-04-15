#include "onig-searcher.h"
#include "unicode-utils.h"

shared_ptr<OnigResult> OnigSearcher::Search(const string& stringToSearch, wchar_t* utf16StringToSearch, bool hasMultibyteCharacters, int charOffset) {
  int byteOffset = charOffset;
  if (hasMultibyteCharacters) {
#ifdef _WIN32
    byteOffset = UnicodeUtils::bytes_in_characters(utf16StringToSearch, charOffset);
#else
    byteOffset = UnicodeUtils::bytes_in_characters(stringToSearch.data(), charOffset);
#endif
  }

  int bestLocation = 0;
  shared_ptr<OnigResult> bestResult;
  cache.Init(stringToSearch, byteOffset);

  vector< shared_ptr<OnigRegExp> >::iterator iter = regExps.begin();
  while (iter < regExps.end()) {
    OnigRegExp *regExp = (*iter).get();
    shared_ptr<OnigResult> result = cache.Search(regExp, stringToSearch, byteOffset);
    if (result != NULL && result->Count() > 0) {
      int location = result->LocationAt(0);
      if (hasMultibyteCharacters) {
        location =  UnicodeUtils::characters_in_bytes(stringToSearch.data(), location);
      }

      if (bestResult == NULL || location < bestLocation) {
        bestLocation = location;
        bestResult = result;
      }

      if (location == charOffset) {
        break;
      }
    }

    ++iter;
  }

  return bestResult;
}
