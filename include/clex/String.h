#ifndef CLEX_STRING_H
#define CLEX_STRING_H

#include "clex/Memory.h"

#include <stddef.h>
#include <string.h>

struct String {
  size_t capacity;
  size_t size;
  char *data;
};

struct String *CreateString(size_t capacity);

void DestroyString(struct String **str);

void ResizeString(struct String *str, size_t newCapacity);

static inline const char *StringEnd(struct String *str) {
  return str->data + str->size;
}

#endif // CLEX_STRING_H
