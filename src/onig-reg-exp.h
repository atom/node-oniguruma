#ifndef SRC_ONIG_REG_EXP_H_
#define SRC_ONIG_REG_EXP_H_

#include <memory>
#include <string>
#include <vector>

#include "oniguruma.h"

using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

class OnigResult;

class OnigRegExp {
  public:
    explicit OnigRegExp(const string& source, int indexInScanner);
    ~OnigRegExp();

    bool Contains(const string& value);
    int LocationAt(int index);
    int Index() { return indexInScanner; }
    shared_ptr<OnigResult> Search(const string &searchString, size_t position);

  private:
    static int name_callback(const UChar* name, const UChar* name_end, int ngroup_num, int* group_nums, regex_t* regex, void* _this);

    OnigRegExp(const OnigRegExp&);  // Disallow copying
    OnigRegExp &operator=(const OnigRegExp&);  // Disallow copying

    string source_;
    vector<string> group_names_;
    regex_t* regex_;
    int indexInScanner;
};

#endif  // SRC_ONIG_REG_EXP_H_
