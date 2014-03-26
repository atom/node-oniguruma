#ifndef SRC_ONIG_SCANNER_WORKER_H_
#define SRC_ONIG_SCANNER_WORKER_H_

#include <node.h>
#include <string>
#include <vector>
#include "nan.h"
#include "onig-reg-exp.h"
#include "onig-result.h"
#include "onig-searcher.h"
#include "unicode-utils.h"

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
                    int charOffset)
    : NanAsyncWorker(callback),
      regExps(regExps),
      stringToSearch(stringToSearch),
      utf16StringToSearch(utf16StringToSearch),
      hasMultibyteCharacters(hasMultibyteCharacters),
      charOffset(charOffset) {
    searcher = shared_ptr<OnigSearcher>(new OnigSearcher(regExps));
  }

  ~OnigScannerWorker() {}

  void Execute();
  void HandleOKCallback();

 private:
  vector<shared_ptr<OnigRegExp>> regExps;
  string stringToSearch;
  shared_ptr<wchar_t> utf16StringToSearch;
  bool hasMultibyteCharacters;
  int charOffset;
  shared_ptr<OnigSearcher> searcher;
  shared_ptr<OnigResult> bestResult;
};

#endif  // SRC_ONIG_SCANNER_WORKER_H_
