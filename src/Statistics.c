#include "clex/Statistics.h"

#include <stdio.h>

static size_t NumOfIdentifier = 0;
static size_t NumOfNumeric = 0;
static size_t NumOfCharConstant = 0;
static size_t NumOfWideCharConstant = 0;
static size_t NumOfStringLiteral = 0;
static size_t NumOfWideStringLiteral = 0;
static size_t NumOfPunctuator = 0;
static size_t NumOfKeyword = 0;

void ClearStatistics() {
  NumOfIdentifier = 0;
  NumOfNumeric = 0;
  NumOfCharConstant = 0;
  NumOfWideCharConstant = 0;
  NumOfStringLiteral = 0;
  NumOfWideStringLiteral = 0;
  NumOfPunctuator = 0;
  NumOfKeyword = 0;
}

void AddTokenToStatistics(const struct Token *tok) {
  if (tok == NULL)
    return;

  switch (tok->kind) {
  case TOKEN_identifier:
    NumOfIdentifier++;
    break;

  case TOKEN_numeric_constant:
    NumOfNumeric++;
    break;

  case TOKEN_char_constant:
    NumOfCharConstant++;
    break;

  case TOKEN_wide_char_constant:
    NumOfWideCharConstant++;
    break;

  case TOKEN_string_literal:
    NumOfStringLiteral++;
    break;

  case TOKEN_wide_string_literal:
    NumOfWideStringLiteral++;
    break;

  default:
    if (GetPunctuatorSpelling(tok->kind) != NULL) {
      NumOfPunctuator++;
    } else if (GetKeywordSpelling(tok->kind) != NULL) {
      NumOfKeyword++;
    }
  }
}

void PrintStatistics() {
  printf("Statistics:\n"
         "  Identifier:  %zu\n"
         "  Numeric:     %zu\n"
         "  Char:        %zu\n"
         "  Wide Char:   %zu\n"
         "  String:      %zu\n"
         "  Wide String: %zu\n"
         "  Punctuator:  %zu\n"
         "  Keyword:     %zu\n",
         NumOfIdentifier,
         NumOfNumeric,
         NumOfCharConstant,
         NumOfWideCharConstant,
         NumOfStringLiteral,
         NumOfWideStringLiteral,
         NumOfPunctuator,
         NumOfKeyword);
}
