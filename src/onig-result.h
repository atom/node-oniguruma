#ifndef ONIG_RESULT_H_
#define ONIG_RESULT_H_

#include <string>

using namespace std;

class OnigRegExp;

class OnigResult {
  public:
    OnigResult(OnigRegion* region, const string& searchString);
    ~OnigResult();

    int Count();
    int LocationAt(int index);
    int LengthAt(int index);

  private:
    OnigResult(const OnigResult&); // Disallow copying
    OnigResult &operator=(const OnigResult&);  // Disallow copying

    string searchString_;
    OnigRegion *region_;
};

#endif // ONIG_RESULT_H_
