#include "onig-searcher.h"

shared_ptr<OnigResult> OnigSearcher::Search(OnigString* source, int charOffset) {
  int bestLocation = 0;
  shared_ptr<OnigResult> bestResult;

  vector< shared_ptr<OnigRegExp> >::iterator iter = regExps.begin();
  int index = 0;
  while (iter < regExps.end()) {
    OnigRegExp *regExp = (*iter).get();
    shared_ptr<OnigResult> result = regExp->Search(source, charOffset);
    if (result != NULL && result->Count() > 0) {
      int location = result->LocationAt(0);

      if (bestResult == NULL || location < bestLocation) {
        bestLocation = location;
        bestResult = result;
        bestResult->SetIndex(index);
      }

      if (location == charOffset) {
        break;
      }
    }
    ++index;
    ++iter;
  }

  return bestResult;
}
