#include "clex/Lexer.h"

#include <assert.h>

static void SkipLineComment(struct SourceManager *srcMgr);

static bool SkipBlockComment(struct SourceManager *srcMgr);

static bool MatchPunctuator(struct SourceManager *srcMgr, struct Token *tok);

static bool MatchNumericConstant(struct SourceManager *srcMgr,
                                 struct Token *tok);

static bool MatchCharConstant(struct SourceManager *srcMgr, struct Token *tok);

static bool MatchWideCharConstant(struct SourceManager *srcMgr,
                                  struct Token *tok);

static bool MatchStringLiteral(struct SourceManager *srcMgr, struct Token *tok);

static bool MatchWideStringLiteral(struct SourceManager *srcMgr,
                                   struct Token *tok);

static bool MatchIdentifierOrKeyword(struct SourceManager *srcMgr,
                                     struct Token *tok);

bool GetToken(struct SourceManager *srcMgr, struct Token *tok) {
  assert(srcMgr != NULL);
  assert(tok != NULL);

  // skip whitespace and comment
  while (1) {
    Utf8Char c = SourceManagerCurrentChar(srcMgr);
    if (c.c[0] == '\0')
      break;
    if (Utf8CharIsSpace(c))
      continue;
    if (c.c[0] == '/') {
      if (SourceManagerIsValidCursor(srcMgr, srcMgr->cursor + 1)) {
        if (srcMgr->cursor[1] == '/')
          SkipLineComment(srcMgr);
        else if (srcMgr->cursor[1] == '*') {
          bool res = SkipBlockComment(srcMgr);
          if (!res) // unclosed block comment.
            return res;
        }
      }
      break; // found non-whitespace character
    }
    break;
  }

  tok->row = srcMgr->row;
  tok->col = srcMgr->col;
  tok->startOfLine = srcMgr->cursor;
  tok->source.data = srcMgr->cursor;
  tok->kind = TOKEN_unknown;

  return true;
}
