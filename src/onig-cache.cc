#include "onig-cache.h"
#include "unicode-utils.h"

void OnigCache::Clear() {
  maxCachedIndex = -1;
  results.clear();
  results.resize(maxSize);
}

void OnigCache::Init(const string &stringToSearch, int byteOffset) {
  useCache = (stringToSearch == lastMatchedString && byteOffset >= lastStartLocation);
  lastStartLocation = byteOffset;

  if (!useCache) {
    Clear();
    lastMatchedString = stringToSearch;
  }
}

void OnigCache::Reset(const OnigCache& cache) {
  lastMatchedString = cache.lastMatchedString;
  maxCachedIndex = cache.maxCachedIndex;
  lastStartLocation = cache.lastStartLocation;
  results = cache.results;
}

shared_ptr<OnigResult> OnigCache::Search(OnigRegExp *regExp, const string &searchString, int byteOffset) {
  shared_ptr<OnigResult> result;
  int index = regExp->Index();
  bool useCachedResult = false;

  if (useCache && index <= maxCachedIndex) {
    result = results[index];
    useCachedResult = result == NULL || result->LocationAt(0) >= byteOffset;
  }

  if (!useCachedResult) {
    result = regExp->Search(searchString, byteOffset);
    results[index] = result;
    maxCachedIndex = index;
  }

  return result;
}
