#include "clex/SourceManager.h"

struct SourceManager *CreateSourceManager(struct StringPiece src) {
  struct SourceManager *srcMgr =
      (struct SourceManager *)Allocate(sizeof(struct SourceManager));
  if (srcMgr == NULL)
    return NULL;
  srcMgr->source = src;
  srcMgr->startOfLine = src.data;
  srcMgr->cursor = src.data;
  srcMgr->row = 1;
  srcMgr->col = 1;
  srcMgr->numOfChar = 0;
  return srcMgr;
}

void DestroySourceManager(struct SourceManager **srcMgr) {
  if (srcMgr == NULL || (*srcMgr) == NULL)
    return;
  Deallocate(*srcMgr);
  *srcMgr = NULL;
}

Utf8Char SourceManagerCurrentChar(struct SourceManager *srcMgr) {
  if (srcMgr == NULL)
    return UTF8_0;
  if (srcMgr->cursor < srcMgr->source.data + srcMgr->source.size)
    return Utf8CharFromStr(srcMgr->cursor);
  else
    return UTF8_0;
}

Utf8Char SourceManagerGetChar(struct SourceManager *srcMgr) {
  if (srcMgr == NULL)
    return UTF8_0;
  if (srcMgr->cursor < srcMgr->source.data + srcMgr->source.size) {
    Utf8Char u8c = SourceManagerCurrentChar(srcMgr);
    srcMgr->cursor = NextUtf8ConstCharPtr(srcMgr->cursor);

    if (u8c.c[0] == '\n') {
      srcMgr->row += 1;
      srcMgr->col = 1;
      srcMgr->startOfLine = srcMgr->cursor;
    } else {
      srcMgr->col += 1;
    }

    srcMgr->numOfChar += 1;
    return u8c;
  } else {
#ifdef DEBUG
    printf("Getchar: NULL\n");
#endif
    return UTF8_0;
  }
}
