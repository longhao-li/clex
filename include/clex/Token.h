#ifndef CLEX_TOKEN_H
#define CLEX_TOKEN_H

#include "clex/StringPiece.h"
#include "clex/TokenKinds.h"

#include <stddef.h>

struct Token {
  enum TokenKind kind;
  size_t row;
  size_t col;
  const char *startOfLine;
  struct StringPiece source;
};

static inline bool IsIdentifierToken(struct Token *tok) {
  if (tok == NULL)
    return false;
  return tok->kind == TOKEN_identifier;
}

static inline bool IsStringLiteralToken(struct Token *tok) {
  if (tok == NULL)
    return false;
  return tok->kind == TOKEN_string_literal ||
         tok->kind == TOKEN_wide_string_literal;
}

static inline bool IsLiteralToken(struct Token *tok) {
  if (tok == NULL)
    return false;
  return tok->kind == TOKEN_numeric_constant ||
         tok->kind == TOKEN_char_constant ||
         tok->kind == TOKEN_wide_char_constant || IsStringLiteralToken(tok);
}

#endif // CLEX_TOKEN_H
