#ifndef SRC_ONIG_RESULT_H_
#define SRC_ONIG_RESULT_H_

#include <string>

using ::std::string;

class OnigRegExp;

class OnigResult {
  public:
    explicit OnigResult(OnigRegion* region);
    ~OnigResult();

    int Count();
    int LocationAt(int index);
    int LengthAt(int index);

  private:
    OnigResult(const OnigResult&);  // Disallow copying
    OnigResult &operator=(const OnigResult&);  // Disallow copying

    OnigRegion *region_;
};

#endif  // SRC_ONIG_RESULT_H_
