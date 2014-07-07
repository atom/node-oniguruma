#include "nan.h"

#include "onig-reg-exp.h"
#include "onig-result.h"

using ::v8::Exception;
using ::v8::String;

OnigRegExp::OnigRegExp(const string& source, int indexInScanner)
    : source_(source),
      group_names_(),
      regex_(NULL),
      indexInScanner(indexInScanner) {
  OnigErrorInfo error;
  const UChar* sourceData = (const UChar*)source.data();
  int status = onig_new(&regex_, sourceData, sourceData + source.length(),
                        ONIG_OPTION_CAPTURE_GROUP, ONIG_ENCODING_UTF8,
                        ONIG_SYNTAX_DEFAULT, &error);

  if (status != ONIG_NORMAL) {
    UChar errorString[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str(errorString, status, &error);
    ThrowException(Exception::Error(String::New(reinterpret_cast<char*>(errorString))));
    return;
  }

  onig_foreach_name(regex_, &OnigRegExp::name_callback, this);
}

/* static */
int OnigRegExp::name_callback(const UChar* name, const UChar* name_end, int ngroup_num, int* group_nums, regex_t* regex, void* _that) {
  OnigRegExp *that = static_cast<OnigRegExp*>(_that);

  for (int i = 0; i < ngroup_num; i++) {
    int group = *(group_nums + i);
    if (static_cast<int>(that->group_names_.size()) < (group + 1)) {
      that->group_names_.resize(group + 1);
    }
    that->group_names_[group] = std::string((const char *)name);
  }

  return 0;
}

OnigRegExp::~OnigRegExp() {
  if (regex_) onig_free(regex_);
}

bool OnigRegExp::Contains(const string& value) {
  return source_.find(value) != string::npos;
}

shared_ptr<OnigResult> OnigRegExp::Search(const string& searchString,
                                          size_t position) {
  if (!regex_) {
    ThrowException(Exception::Error(String::New("RegExp is not valid")));
    return shared_ptr<OnigResult>();
  }

  int end = searchString.size();
  OnigRegion* region = onig_region_new();
  const UChar* searchData = (const UChar*)searchString.data();
  int status = onig_search(regex_, searchData, searchData + end,
                           searchData + position, searchData + end, region,
                           ONIG_OPTION_NONE);

  if (status != ONIG_MISMATCH) {
    return shared_ptr<OnigResult>(new OnigResult(region, group_names_, indexInScanner));
  } else {
    onig_region_free(region, 1);
    return shared_ptr<OnigResult>();
  }
}
