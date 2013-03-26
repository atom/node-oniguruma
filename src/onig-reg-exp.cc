#include <v8.h>
#include "onig-reg-exp.h"
#include "onig-result.h"

using namespace v8;

OnigRegExp::OnigRegExp(const string& source) : source_(source) {
  OnigErrorInfo error;
  const UChar* sourceData = (const UChar*)source.data();
  int status = onig_new(&regex_, sourceData, sourceData + source.length(),
                        ONIG_OPTION_NONE, ONIG_ENCODING_UTF8,
                        ONIG_SYNTAX_DEFAULT, &error);

  if (status == ONIG_NORMAL) {
    return;
  }
  else {
    UChar errorString[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str(errorString, status, &error);
    ThrowException(Exception::Error(String::New((char*)errorString)));
  }
}

OnigRegExp::~OnigRegExp() {
  if (regex_) onig_free(regex_);
}

bool OnigRegExp::Contains(const string& value) {
  return source_.find(value) != string::npos;
}

OnigResult* OnigRegExp::Search(const string& searchString, size_t position) {
  int end = searchString.size();
  OnigRegion* region = onig_region_new();
  const UChar* searchData = (const UChar*)searchString.data();
  int status = onig_search(regex_, searchData, searchData + end,
                           searchData + position, searchData + end, region,
                           ONIG_OPTION_NONE);

  if (status != ONIG_MISMATCH) {
    return new OnigResult(region, searchString);
  }
  else {
    onig_region_free(region, 1);
    return nullptr;
  }
}
