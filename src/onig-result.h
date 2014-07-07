#ifndef SRC_ONIG_RESULT_H_
#define SRC_ONIG_RESULT_H_

#include <string>
#include <vector>

using ::std::string;
using ::std::vector;

class OnigRegExp;

class OnigResult {
  public:
    explicit OnigResult(OnigRegion* region, const vector<string>& group_names, int indexInScanner);
    ~OnigResult();

    int Count();
    int LocationAt(int index);
    int LengthAt(int index);
    string NameAt(int index);
    int Index() { return indexInScanner; }

  private:
    OnigResult(const OnigResult&);  // Disallow copying
    OnigResult &operator=(const OnigResult&);  // Disallow copying

    OnigRegion *region_;
    vector<string> group_names_;
    int indexInScanner;
};

#endif  // SRC_ONIG_RESULT_H_
