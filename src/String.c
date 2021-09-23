#include "clex/String.h"

struct String *CreateString(size_t capacity) {
  struct String *str = (struct String *)Allocate(sizeof(struct String));
  str->data = (char *)Allocate(capacity + 1);
  str->capacity = capacity;
  str->size = 0;
  return str;
}

void DestroyString(struct String **str) {
  if (str == NULL || (*str) == NULL)
    return;
  struct String *p = (*str);
  *str = NULL;
  Deallocate(p->data);
  Deallocate(p);
}

void ResizeString(struct String *str, size_t newCapacity) {
  if (newCapacity > str->capacity)
    return;

  char *newData = (char *)Allocate(newCapacity + 1);
  memset(newData, 0, newCapacity + 1);
  memcpy(newData, str->data, str->size);

  Deallocate(str->data);
  str->data = newData;
}

