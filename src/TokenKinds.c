#include "clex/TokenKinds.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

static const char *const TokenNames[] = {
#define TOK(X) #X,
#define KEYWORD(X, Y) #X,
#include "clex/TokenKinds.def"
    NULL};

static const char *const KeywordNames[] = {
#define KEYWORD(X, Y) #X,
#include "clex/TokenKinds.def"
};

const char *GetTokenName(enum TokenKind kind) {
  assert(kind >= 0 && kind < TOKEN_NUM_TOKENS);
  return TokenNames[kind];
}

const char *GetPunctuatorSpelling(enum TokenKind kind) {
  switch (kind) {
#define PUNCTUATOR(X, Y)                                                       \
  case TOKEN_##X:                                                              \
    return Y;
#include "clex/TokenKinds.def"
  default:
    break;
  }
  return NULL;
}

const char *GetKeywordSpelling(enum TokenKind kind) {
  switch (kind) {
#define KEYWORD(X, Y)                                                          \
  case TOKEN_kw_##X:                                                           \
    return #X;
#include "clex/TokenKinds.def"
  default:
    break;
  }
  return NULL;
}

enum TokenKind StringPieceToKeywordType(struct StringPiece str) {
  for (size_t i = 0; i < sizeof(KeywordNames) / sizeof(char *); i++) {
    if (strlen(KeywordNames[i]) != str.size)
      continue;
    if (memcmp(str.data, KeywordNames[i], str.size) == 0)
      return TOKEN_kw_auto + i;
  }
  return TOKEN_unknown;
}
