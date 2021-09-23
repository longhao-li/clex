#ifndef CLEX_STRING_PIECE_H
#define CLEX_STRING_PIECE_H

#include "clex/String.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct StringPiece {
  const char *data;
  size_t size;
};

static inline struct StringPiece StringPieceFromString(struct String *str) {
  struct StringPiece strRef = {NULL, 0};
  if (str != NULL) {
    strRef.data = str->data;
    strRef.size = str->size;
  }
  return strRef;
}

static inline struct StringPiece StringPieceFromRawStr(const char *str) {
  struct StringPiece strRef = {NULL, 0};
  if (str != NULL) {
    strRef.data = str;
    strRef.size = strlen(str);
  }
  return strRef;
}

static inline void PrintStringPiece(struct StringPiece str) {
  for (size_t i = 0; i < str.size; i++)
    putchar(str.data[i]);
}

#endif // CLEX_STRING_PIECE_H