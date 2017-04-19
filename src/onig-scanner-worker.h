#ifndef SRC_ONIG_SCANNER_WORKER_H_
#define SRC_ONIG_SCANNER_WORKER_H_

#include <memory>
#include <vector>

#include "nan.h"
#include "onig-reg-exp.h"
#include "onig-searcher.h"

class OnigScannerWorker : public Nan::AsyncWorker {
 public:
  OnigScannerWorker(Nan::Callback *callback,
                    std::vector<std::shared_ptr<OnigRegExp>> regExps,
                    v8::Local<v8::String> v8String,
                    int charOffset)
    : Nan::AsyncWorker(callback),
      charOffset(charOffset) {
        source = new OnigString(v8String);
    searcher = shared_ptr<OnigSearcher>(new OnigSearcher(regExps));
  }

  ~OnigScannerWorker() {
    delete source;
  }

  void Execute();
  void HandleOKCallback();

 private:
  OnigString* source;
  int charOffset;
  std::shared_ptr<OnigSearcher> searcher;
  std::shared_ptr<OnigResult> bestResult;
};

#endif  // SRC_ONIG_SCANNER_WORKER_H_
