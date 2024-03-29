#ifndef CLEX_TOKEN_KINDS_H
#define CLEX_TOKEN_KINDS_H

#include "clex/Bool.h"
#include "clex/StringPiece.h"

enum {
  KEYC99 = 0x1,
  KEYC11 = 0x2,
  KEYC17 = 0x4,
  KEYC23 = 0x8,
  KEYALL = KEYC99 | KEYC11 | KEYC17 | KEYC23,
};

enum TokenKind {
#define TOK(X) TOKEN_##X,
#include "clex/TokenKinds.def"
  TOKEN_NUM_TOKENS
};

const char *GetTokenName(enum TokenKind kind);

const char *GetPunctuatorSpelling(enum TokenKind kind);

const char *GetKeywordSpelling(enum TokenKind kind);

enum TokenKind StringPieceToKeywordType(struct StringPiece str);

#endif // CLEX_TOKEN_KINDS_H
