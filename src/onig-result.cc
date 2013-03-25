#include "oniguruma.h"
#include "onig-result.h"
#include "unicode-utils.h"

OnigResult::OnigResult(OnigRegion* region, const string& searchString) : searchString_(searchString) {
  region_ = region;
}

OnigResult::~OnigResult() {
  onig_region_free(region_, 1);
}

int OnigResult::Count() {
  return region_->num_regs;
}

int OnigResult::LocationAt(int index) {
  int bytes = *(region_->beg + index);
  return UnicodeUtils::characters_in_bytes(searchString_.data(), bytes);
}

int OnigResult::LengthAt(int index) {
  int bytes = *(region_->end + index) - *(region_->beg + index);
  const char *search = searchString_.data() + *(region_->beg + index);
  return UnicodeUtils::characters_in_bytes(search, bytes);
}
