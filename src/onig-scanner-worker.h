#ifndef SRC_ONIG_SCANNER_WORKER_H_
#define SRC_ONIG_SCANNER_WORKER_H_

#include <node.h>
#include <string>
#include <vector>
#include "nan.h"
#include "onig-reg-exp.h"
#include "onig-result.h"
#include "unicode-utils.h"

using ::std::string;
using ::std::shared_ptr;
using ::std::vector;

class OnigScannerWorker : public NanAsyncWorker {
 public:
  OnigScannerWorker(NanCallback *callback,
                    vector<shared_ptr<OnigRegExp>> regExps,
                    vector<shared_ptr<OnigResult>> cachedResults,
                    string lastMatchedString,
                    int maxCachedIndex,
                    int lastStartLocation,
                    bool useCachedResults,
                    string stringToSearch,
                    wchar_t *utf16StringToSearch,
                    bool hasMultibyteCharacters,
                    int charOffset)
    : NanAsyncWorker(callback),
      regExps(regExps),
      cachedResults(cachedResults),
      lastMatchedString(lastMatchedString),
      maxCachedIndex(maxCachedIndex),
      lastStartLocation(lastStartLocation),
      charOffset(charOffset),
      stringToSearch(stringToSearch),
      utf16StringToSearch(utf16StringToSearch),
      hasMultibyteCharacters(hasMultibyteCharacters),
      useCachedResults(useCachedResults),
      bestIndex(-1),
      bestResult(NULL) {}

  ~OnigScannerWorker() {}

  void Execute();
  void HandleOKCallback();

 private:
  vector<shared_ptr<OnigRegExp>> regExps;
  vector<shared_ptr<OnigResult>> cachedResults;
  string lastMatchedString;
  int maxCachedIndex;
  int lastStartLocation;
  int charOffset;
  string stringToSearch;
  shared_ptr<wchar_t> utf16StringToSearch;
  bool hasMultibyteCharacters;
  bool useCachedResults;
  int bestIndex;
  shared_ptr<OnigResult> bestResult;
};

#endif  // SRC_ONIG_SCANNER_WORKER_H_
