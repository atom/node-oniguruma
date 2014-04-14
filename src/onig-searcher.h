#ifndef SRC_ONIG_SEARCHER_H_
#define SRC_ONIG_SEARCHER_H_

#include <string>
#include <vector>
#include "onig-cache.h"
#include "onig-reg-exp.h"
#include "onig-result.h"

using ::std::string;
using ::std::shared_ptr;
using ::std::vector;

class OnigSearcher {
 public:
  explicit OnigSearcher(vector<shared_ptr<OnigRegExp>> regExps)
    : regExps(regExps),
      cache(regExps.size()) {}

  ~OnigSearcher() {}

  shared_ptr<OnigResult> Search(string stringToSearch, wchar_t* utf16StringToSearch, bool hasMultibyteCharacters, int charOffset);

 private:
  vector<shared_ptr<OnigRegExp>> regExps;
  OnigCache cache;
};

#endif  // SRC_ONIG_SEARCHER_H_
