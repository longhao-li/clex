#include "clex/Utf8Char.h"

size_t Utf8CharSize(char c) {
  if ((c & 0xf0) == 0xf0)
    return 4;
  else if ((c & 0xe0) == 0xe0)
    return 3;
  else if ((c & 0xc0) == 0xc0)
    return 2;
  else
    return 1;
}

Utf8Char Utf8CharFromStr(const char *str) {
  if (str == NULL)
    return UTF8_0;

  size_t len = Utf8CharSize(str[0]);
  Utf8Char u8c = UTF8_0;

  for (size_t i = 0; i < len; i++)
    u8c.c[i] = str[i];
  return u8c;
}

char *NextUtf8CharPtr(char *cur) {
  if (cur == NULL)
    return NULL;
  if (cur[0] == '0')
    return NULL;

  return cur + Utf8CharSize(cur[0]);
}

const char *NextUtf8ConstCharPtr(const char *cur) {
  if (cur == NULL)
    return NULL;
  if (cur[0] == '0')
    return NULL;

  return cur + Utf8CharSize(cur[0]);
}
