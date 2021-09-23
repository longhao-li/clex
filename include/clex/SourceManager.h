#ifndef CLEX_SOURCE_MANAGER_H
#define CLEX_SOURCE_MANAGER_H

#include "clex/Memory.h"
#include "clex/StringPiece.h"
#include "clex/Utf8Char.h"

#include <assert.h>

struct SourceManager {
  struct StringPiece source;
  const char *startOfLine;
  const char *cursor;
  size_t row;
  size_t col;
  size_t numOfChar; // used for statistics
};

struct SourceManager *CreateSourceManager(struct StringPiece src);

void DestroySourceManager(struct SourceManager **srcMgr);

static inline size_t SourceManagerLine(struct SourceManager *srcMgr) {
  return srcMgr == NULL ? 0 : srcMgr->row;
}

static inline size_t SourceManagerColumn(struct SourceManager *srcMgr) {
  return srcMgr == NULL ? 0 : srcMgr->col;
}

static inline bool SourceManagerIsValidCursor(struct SourceManager *srcMgr,
                                              const char *cur) {
  return cur >= srcMgr->source.data &&
         cur < srcMgr->source.data + srcMgr->source.size;
}

Utf8Char SourceManagerCurrentChar(struct SourceManager *srcMgr);

Utf8Char SourceManagerGetChar(struct SourceManager *srcMgr);

#endif // CLEX_SOURCE_MANAGER_H
