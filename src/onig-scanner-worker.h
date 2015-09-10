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

class OnigScannerWorker : public Nan::AsyncWorker {
 public:
  OnigScannerWorker(Nan::Callback *callback,
                    vector<shared_ptr<OnigRegExp>> regExps,
                    shared_ptr<OnigStringContext> source,
                    int charOffset,
                    shared_ptr<OnigCache> cache)
    : Nan::AsyncWorker(callback),
      source(source),
      charOffset(charOffset),
      cache(cache) {
    searcher = shared_ptr<OnigSearcher>(new OnigSearcher(regExps, *cache.get()));
  }

  ~OnigScannerWorker() {}

  void Execute();
  void HandleOKCallback();

 private:
  shared_ptr<OnigStringContext> source;
  int charOffset;
  shared_ptr<OnigCache> cache;
  shared_ptr<OnigSearcher> searcher;
  shared_ptr<OnigResult> bestResult;
};

#endif  // SRC_ONIG_SCANNER_WORKER_H_
