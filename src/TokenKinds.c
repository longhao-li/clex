#include "clex/TokenKinds.h"

#include <assert.h>
#include <stddef.h>

static const char *const TokenNames[] = {
#define TOK(X) #X,
#define KEYWORD(X, Y) #X,
#include "clex/TokenKinds.def"
    NULL};

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
