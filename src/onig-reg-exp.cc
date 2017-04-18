#include "onig-reg-exp.h"

using ::v8::Exception;
using ::v8::String;

OnigRegExp::OnigRegExp(const OnigString &source) : regex_(NULL) {
  lastSearchStrUniqueId = -1;
  lastSearchPosition = -1;
  lastSearchResult = NULL;

  OnigErrorInfo error;

  const UChar* sourceStart = reinterpret_cast<const UChar *>(source.content.data());
  const UChar* sourceEnd = reinterpret_cast<const UChar *>(source.content.data() + source.content.size());
  int status = onig_new(&regex_, sourceStart, sourceEnd,
                        ONIG_OPTION_CAPTURE_GROUP, ONIG_ENCODING_UTF16_LE,
                        ONIG_SYNTAX_DEFAULT, &error);

  if (status != ONIG_NORMAL) {
    UChar errorString[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str(errorString, status, &error);
    Nan::ThrowError(Exception::Error(Nan::New<String>(reinterpret_cast<char*>(errorString)).ToLocalChecked()));
  }
}

OnigRegExp::~OnigRegExp() {
  if (regex_) onig_free(regex_);
}

shared_ptr<OnigResult> OnigRegExp::Search(OnigString* str, int position) {
  if (lastSearchStrUniqueId == str->uniqueId && lastSearchPosition <= position) {
    if (lastSearchResult == NULL || lastSearchResult->LocationAt(0) >= position) {
      return lastSearchResult;
    }
  }

  lastSearchStrUniqueId = str->uniqueId;
  lastSearchPosition = position;

  if (!regex_) {
    Nan::ThrowError(Exception::Error(Nan::New<String>("RegExp is not valid").ToLocalChecked()));
    return shared_ptr<OnigResult>();
  }

  const uint16_t *data = str->content.data();
  const UChar* searchStart = reinterpret_cast<const UChar*>(data);
  const UChar* searchPosition = reinterpret_cast<const UChar*>(data + position);
  const UChar* searchEnd = reinterpret_cast<const UChar*>(data + str->content.size());

  OnigRegion* region = onig_region_new();
  int status = onig_search(
    regex_, searchStart, searchEnd, searchPosition, searchEnd, region,
    ONIG_OPTION_NONE
  );

  if (status != ONIG_MISMATCH) {
    lastSearchResult = shared_ptr<OnigResult>(new OnigResult(region, -1));
  } else {
    onig_region_free(region, 1);
    lastSearchResult = shared_ptr<OnigResult>();
  }

  return lastSearchResult;
}
