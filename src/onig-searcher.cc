#include "onig-searcher.h"
#include "unicode-utils.h"

void OnigSearcher::ClearCachedResults() {
  maxCachedIndex = -1;
  cachedResults.clear();
}

bool OnigSearcher::UseCachedResults(string stringToSearch, int charOffset) {
  bool useCachedResults = (stringToSearch == lastMatchedString && charOffset >= lastStartLocation);
  lastStartLocation = charOffset;

  if (!useCachedResults) {
    ClearCachedResults();
    lastMatchedString = stringToSearch;
  }

  return useCachedResults;
}

shared_ptr<OnigResult> OnigSearcher::Search(string stringToSearch, wchar_t* utf16StringToSearch, bool hasMultibyteCharacters, int charOffset) {
  int byteOffset = charOffset;
  if (hasMultibyteCharacters) {
#ifdef _WIN32
    byteOffset = UnicodeUtils::bytes_in_characters(utf16StringToSearch.get(), charOffset);
#else
    byteOffset = UnicodeUtils::bytes_in_characters(stringToSearch.data(), charOffset);
#endif
  }

  int bestLocation = 0;
  shared_ptr<OnigResult> bestResult;
  bool useCachedResults = UseCachedResults(stringToSearch, charOffset);

  vector< shared_ptr<OnigRegExp> >::iterator iter = regExps.begin();
  int index = 0;
  while (iter < regExps.end()) {
    OnigRegExp *regExp = (*iter).get();

    bool useCachedResult = false;
    shared_ptr<OnigResult> result;

    if (useCachedResults && index <= maxCachedIndex) {
      result = cachedResults[index];
      if (result != NULL) {
        int location = result->LocationAt(0);
        if (hasMultibyteCharacters) {
          location = UnicodeUtils::characters_in_bytes(stringToSearch.data(), location);
        }
        useCachedResult = location >= charOffset;
      } else {
        useCachedResult = true;
      }
    }

    if (!useCachedResult) {
      result = regExp->Search(stringToSearch, byteOffset);
      cachedResults[index] = result;
      maxCachedIndex = index;
    }

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
    index++;
  }

  return bestResult;
}
