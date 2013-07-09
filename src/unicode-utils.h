#ifndef SRC_UNICODE_UTILS_H_
#define SRC_UNICODE_UTILS_H_

#include <string>

class UnicodeUtils {
  public:
    static int characters_in_bytes(const char* string, int bytes);
    static int bytes_in_characters(const char* string, int characters);
};

#endif  // SRC_UNICODE_UTILS_H_
