#ifndef SRC_ONIG_REG_EXP_H_
#define SRC_ONIG_REG_EXP_H_

#include <memory>
#include <string>

#include "oniguruma.h"

using ::std::shared_ptr;
using ::std::string;

class OnigResult;

class OnigRegExp {
  public:
    explicit OnigRegExp(const string& source);
    ~OnigRegExp();

    bool Contains(const string& value);
    int LocationAt(int index);
    shared_ptr<OnigResult> Search(const string &searchString, size_t position);

  private:
    OnigRegExp(const OnigRegExp&);  // Disallow copying
    OnigRegExp &operator=(const OnigRegExp&);  // Disallow copying

    string source_;
    regex_t* regex_;
};

#endif  // SRC_ONIG_REG_EXP_H_
