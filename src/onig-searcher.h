#ifndef SRC_ONIG_SEARCHER_H_
#define SRC_ONIG_SEARCHER_H_

#include <string>
#include <vector>
#include "onig-reg-exp.h"
#include "onig-result.h"

using ::std::string;
using ::std::shared_ptr;
using ::std::vector;

class OnigSearcher {
 public:
  explicit OnigSearcher(vector<shared_ptr<OnigRegExp>> regExps)
    : regExps(regExps),
      maxCachedIndex(-1),
      lastStartLocation(-1) {
    cachedResults.resize(regExps.size());
  }

  ~OnigSearcher() {}

  shared_ptr<OnigResult> Search(string stringToSearch, wchar_t* utf16StringToSearch, bool hasMultibyteCharacters, int charOffset);

 private:
  void ClearCachedResults();
  bool UseCachedResults(string stringToSearch, int charOffset);

  vector<shared_ptr<OnigRegExp>> regExps;
  vector<shared_ptr<OnigResult>> cachedResults;
  string lastMatchedString;
  int maxCachedIndex;
  int lastStartLocation;
};

#endif  // SRC_ONIG_SEARCHER_H_
