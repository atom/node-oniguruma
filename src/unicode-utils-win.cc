#include "unicode-utils.h"

#include <windows.h>

int UnicodeUtils::characters_in_bytes(const char* string, int bytes) {
  return MultiByteToWideChar(CP_UTF8, 0, string, bytes, NULL, 0);
}

int UnicodeUtils::bytes_in_characters(const wchar_t* string, int characters) {
  return WideCharToMultiByte(CP_UTF8, 0, string, characters, NULL, 0, NULL, NULL);
}
