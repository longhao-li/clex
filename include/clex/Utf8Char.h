#ifndef CLEX_UTF8_CHAR_H
#define CLEX_UTF8_CHAR_H

#include "clex/Bool.h"

#include <ctype.h>
#include <stddef.h>

typedef struct Utf8Char {
  char c[4];
} Utf8Char;

#define UTF8_0                                                                 \
  (struct Utf8Char) {                                                          \
    { 0, 0, 0, 0 }                                                             \
  }

static inline bool Utf8CharIsAlpha(const Utf8Char c) {
  return isalpha(c.c[0]);
}

static inline bool Utf8CharIsAlnum(const Utf8Char c) {
  return isalnum(c.c[0]);
}

static inline bool Utf8CharIsSpace(const Utf8Char c) {
  return isspace(c.c[0]);
}

static inline bool Utf8CharIsPunct(const Utf8Char c) {
  return ispunct(c.c[0]);
}

static inline bool Utf8CharIsDigit(const Utf8Char c) {
  return isdigit(c.c[0]);
}

static inline bool Utf8CharIsXDigit(const Utf8Char c) {
  return isxdigit(c.c[0]);
}

size_t Utf8CharSize(char c);

static inline bool Utf8CharIsMultiByte(const Utf8Char c) {
  return Utf8CharSize(c.c[0]) > 1;
}

Utf8Char Utf8CharFromStr(const char *str);

char *NextUtf8CharPtr(char *cur);

const char *NextUtf8ConstCharPtr(const char *cur);

#endif // CLEX_UTF8_CHAR_H
