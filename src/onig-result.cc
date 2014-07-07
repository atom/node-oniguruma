#include "oniguruma.h"
#include "onig-result.h"
#include "unicode-utils.h"

OnigResult::OnigResult(OnigRegion* region, const vector<string>& group_names, int indexInScanner)
  : region_(region),
    group_names_(group_names),
    indexInScanner(indexInScanner)
{ }

OnigResult::~OnigResult() {
  onig_region_free(region_, 1);
}

int OnigResult::Count() {
  return region_->num_regs;
}

int OnigResult::LocationAt(int index) {
  int bytes = *(region_->beg + index);
  if (bytes > 0)
    return bytes;
  else
    return 0;
}

int OnigResult::LengthAt(int index) {
  int bytes = *(region_->end + index) - *(region_->beg + index);
  if (bytes > 0)
    return bytes;
  else
    return 0;
}

string OnigResult::NameAt(int index) {
  if (index + 1 > static_cast<int>(group_names_.size())) {
    return std::string();
  }
  return group_names_[index];
}
