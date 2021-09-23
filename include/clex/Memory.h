#ifndef CLEX_MEMORY_H
#define CLEX_MEMORY_H

#include <stddef.h>
#include <stdlib.h>

static inline void *Allocate(size_t size) { return malloc(size); }

static inline void Deallocate(void *p) { free(p); }

#endif // CLEX_MEMORY_H
