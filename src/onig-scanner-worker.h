#ifndef SRC_ONIG_SCANNER_WORKER_H_
#define SRC_ONIG_SCANNER_WORKER_H_

#include <node.h>
#include <string>
#include <vector>

#include "nan.h"
#include "onig-reg-exp.h"
#include "onig-result.h"
#include "onig-searcher.h"

using ::std::string;
using ::std::shared_ptr;
using ::std::vector;

class OnigScannerWorker : public NanAsyncWorker {
 public:
  OnigScannerWorker(NanCallback *callback,
                    vector<shared_ptr<OnigRegExp>> regExps,
                    string stringToSearch,
                    shared_ptr<wchar_t> utf16StringToSearch,
                    bool hasMultibyteCharacters,
                    int charOffset,
                    shared_ptr<OnigCache> cache)
    : NanAsyncWorker(callback),
      stringToSearch(stringToSearch),
      utf16StringToSearch(utf16StringToSearch),
      hasMultibyteCharacters(hasMultibyteCharacters),
      charOffset(charOffset),
      cache(cache) {
    searcher = shared_ptr<OnigSearcher>(new OnigSearcher(regExps, *cache.get()));
  }

  ~OnigScannerWorker() {}

  void Execute();
  void HandleOKCallback();

 private:
  string stringToSearch;
  shared_ptr<wchar_t> utf16StringToSearch;
  bool hasMultibyteCharacters;
  int charOffset;
  shared_ptr<OnigCache> cache;
  shared_ptr<OnigSearcher> searcher;
  shared_ptr<OnigResult> bestResult;
};

#endif  // SRC_ONIG_SCANNER_WORKER_H_
