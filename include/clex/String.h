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

static inline void StringPushBack(struct String *str, char c) {
  if (str == NULL || str->capacity == 0)
    return;
  if (str->size + 1 > str->capacity)
    ResizeString(str, (str->capacity + 2) * 1.5);
  str->data[str->size++] = c;
  str->data[str->size] = '\0';
}

static inline const char *StringEnd(struct String *str) {
  return str->data + str->size;
}

#endif // CLEX_STRING_H
