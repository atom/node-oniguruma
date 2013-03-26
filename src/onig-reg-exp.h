#ifndef ONIG_REG_EXP_H_
#define ONIG_REG_EXP_H_

#include <string>
#include "oniguruma.h"

using namespace std;

class OnigResult;

class OnigRegExp {
  public:
    OnigRegExp(const string& source);
    ~OnigRegExp();

    bool Contains(const string& value);
    int LocationAt(int index);
    OnigResult *Search(const string &searchString, size_t position);

  private:
    OnigRegExp(const OnigRegExp&); // Disallow copying
    OnigRegExp &operator=(const OnigRegExp&);  // Disallow copying

    string source_;
    regex_t* regex_;
};

#endif // ONIG_REG_EXP_H_
