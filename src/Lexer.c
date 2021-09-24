#include "clex/Lexer.h"
#include "clex/Error.h"

#include <assert.h>

static void SkipLineComment(struct SourceManager *srcMgr);

static bool SkipBlockComment(struct SourceManager *srcMgr, struct Token *tok);

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

static inline bool IsIdentifierOrKeywordUtf8Char(Utf8Char c) {
  return Utf8CharIsAlnum(c) || c.c[0] == '_' || Utf8CharIsMultiByte(c);
}

bool GetToken(struct SourceManager *srcMgr, struct Token *tok) {
  assert(srcMgr != NULL);
  assert(tok != NULL);

  // skip whitespace and comment
  while (true) {
    Utf8Char c = SourceManagerCurrentChar(srcMgr);
    if (c.c[0] == '\0') {
      tok->kind = TOKEN_eof;
      return true; // end of source
    }

    if (Utf8CharIsSpace(c)) {
      SourceManagerGetChar(srcMgr);
      continue;
    }

    if (c.c[0] == '/') {
      if (SourceManagerIsValidCursor(srcMgr, srcMgr->cursor + 1)) {
        if (srcMgr->cursor[1] == '/')
          SkipLineComment(srcMgr);
        else if (srcMgr->cursor[1] == '*') {
          bool res = SkipBlockComment(srcMgr, tok);
          if (!res) // unclosed block comment.
            return res;
          else
            continue;
        }
      }
      break; // found non-whitespace character
    } else {
      break;
    }
  }

  tok->row = srcMgr->row;
  tok->col = srcMgr->col;
  tok->startOfLine = srcMgr->startOfLine;
  tok->source.data = srcMgr->cursor;
  tok->source.size = 0;
  tok->kind = TOKEN_unknown;

  Utf8Char c = SourceManagerCurrentChar(srcMgr);
  assert(!Utf8CharIsSpace(c));
  switch (c.c[0]) {
  case '\"':
    return MatchStringLiteral(srcMgr, tok);

  case '\'':
    return MatchCharConstant(srcMgr, tok);

  case 'L':
    if (SourceManagerIsValidCursor(srcMgr, srcMgr->cursor + 1)) {
      char c2 = srcMgr->cursor[1];
      if (c2 == '\"')
        return MatchWideStringLiteral(srcMgr, tok);
      else if (c2 == '\'')
        return MatchWideCharConstant(srcMgr, tok);
    }
    return MatchIdentifierOrKeyword(srcMgr, tok);

  case '.':
    if (SourceManagerIsValidCursor(srcMgr, srcMgr->cursor + 1)) {
      char ch2 = srcMgr->cursor[1];
      if (isdigit(ch2))
        return MatchNumericConstant(srcMgr, tok);
    }
    return MatchPunctuator(srcMgr, tok);

  default:
    if (IsIdentifierOrKeywordUtf8Char(c))
      return MatchIdentifierOrKeyword(srcMgr, tok);
    else if (Utf8CharIsPunct(c))
      return MatchPunctuator(srcMgr, tok);
    else if (Utf8CharIsDigit(c))
      return MatchNumericConstant(srcMgr, tok);
    else {
      tok->source.size = Utf8CharSize(c.c[0]);
      PrintLexError("unexpected character", "unrecognized character", tok);
      SourceManagerGetChar(srcMgr); // skip this unknown character
      return false;
    }
  }

  return true;
}

static void SkipLineComment(struct SourceManager *srcMgr) {
  {
    assert(srcMgr->cursor[0] == '/' && srcMgr->cursor[1] == '/');
    srcMgr->cursor += 2;
    srcMgr->numOfChar += 2;
  }

  while (SourceManagerCurrentChar(srcMgr).c[0] != '\0' &&
         SourceManagerCurrentChar(srcMgr).c[0] != '\n') {
    SourceManagerGetChar(srcMgr);
  }

  if (SourceManagerCurrentChar(srcMgr).c[0] == '\n')
    SourceManagerGetChar(srcMgr);
}

static bool SkipBlockComment(struct SourceManager *srcMgr, struct Token *tok) {
  assert(srcMgr->cursor[0] == '/' && srcMgr->cursor[1] == '*');
  // used to output error message
  tok->row = srcMgr->row;
  tok->col = srcMgr->col;
  tok->startOfLine = srcMgr->startOfLine;
  tok->source.data = srcMgr->cursor;
  tok->source.size = 2;

  {
    srcMgr->cursor += 2;
    srcMgr->numOfChar += 2;
  }

  while (SourceManagerCurrentChar(srcMgr).c[0] != '\0' &&
         (SourceManagerGetChar(srcMgr).c[0] != '*' ||
          SourceManagerCurrentChar(srcMgr).c[0] != '/'))
    ;

  if (SourceManagerCurrentChar(srcMgr).c[0] == '\0') {
    // unclosed block comment
    PrintLexError("unclosed block comment", "comment begins here", tok);
    return false;
  }

  assert(SourceManagerCurrentChar(srcMgr).c[0] == '/');
  SourceManagerGetChar(srcMgr);
  return true;
}

static bool MatchPunctuator(struct SourceManager *srcMgr, struct Token *tok) {
  assert(Utf8CharIsPunct(SourceManagerCurrentChar(srcMgr)));
  assert(srcMgr != NULL);

  tok->source.size = 1;
  switch (SourceManagerGetChar(srcMgr).c[0]) {
  case '[':
    tok->kind = TOKEN_l_square;
    break;

  case ']':
    tok->kind = TOKEN_r_square;
    break;

  case '(':
    tok->kind = TOKEN_l_paren;
    break;

  case ')':
    tok->kind = TOKEN_r_paren;
    break;

  case '{':
    tok->kind = TOKEN_l_brace;
    break;

  case '}':
    tok->kind = TOKEN_r_brace;
    break;

  case '.':
    tok->kind = TOKEN_period;
    if (SourceManagerIsValidCursor(srcMgr, srcMgr->cursor + 1)) {
      if (srcMgr->cursor[0] == '.' && srcMgr->cursor[1] == '.') {
        SourceManagerGetChar(srcMgr);
        SourceManagerGetChar(srcMgr);
        tok->kind = TOKEN_ellipsis;
        tok->source.size = 3;
      }
    }
    break;

  case '&':
    tok->kind = TOKEN_amp;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_ampequal;
      tok->source.size = 2;
    } else if (SourceManagerCurrentChar(srcMgr).c[0] == '&') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_ampamp;
      tok->source.size = 2;
    }
    break;

  case '*':
    tok->kind = TOKEN_star;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_starequal;
      tok->source.size = 2;
    }
    break;

  case '+':
    tok->kind = TOKEN_plus;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_plusequal;
      tok->source.size = 2;
    } else if (SourceManagerCurrentChar(srcMgr).c[0] == '+') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_plusplus;
      tok->source.size = 2;
    }
    break;

  case '-':
    tok->kind = TOKEN_minus;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_minusequal;
      tok->source.size = 2;
    } else if (SourceManagerCurrentChar(srcMgr).c[0] == '+') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_minusminus;
      tok->source.size = 2;
    } else if (SourceManagerCurrentChar(srcMgr).c[0] == '+') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_arrow;
      tok->source.size = 2;
    }
    break;

  case '~':
    tok->kind = TOKEN_tilde;
    break;

  case '!':
    tok->kind = TOKEN_exclaim;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_exclaimequal;
      tok->source.size = 2;
    }
    break;

  case '/':
    tok->kind = TOKEN_slash;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_slashequal;
      tok->source.size = 2;
    }
    break;

  case '%':
    tok->kind = TOKEN_percent;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_percentequal;
      tok->source.size = 2;
    }
    break;

  case '<':
    tok->kind = TOKEN_less;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_lessequal;
      tok->source.size = 2;
    } else if (SourceManagerCurrentChar(srcMgr).c[0] == '<') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_lessless;
      tok->source.size = 2;
      if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
        SourceManagerGetChar(srcMgr);
        tok->kind = TOKEN_lesslessequal;
        tok->source.size = 3;
      }
    }
    break;

  case '>':
    tok->kind = TOKEN_greater;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_greaterequal;
      tok->source.size = 2;
    } else if (SourceManagerCurrentChar(srcMgr).c[0] == '>') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_greatergreater;
      tok->source.size = 2;
      if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
        SourceManagerGetChar(srcMgr);
        tok->kind = TOKEN_greatergreaterequal;
        tok->source.size = 3;
      }
    }
    break;

  case '^':
    tok->kind = TOKEN_caret;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_caretequal;
      tok->source.size = 2;
    }
    break;

  case '|':
    tok->kind = TOKEN_pipe;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_pipeequal;
      tok->source.size = 2;
    } else if (SourceManagerCurrentChar(srcMgr).c[0] == '&') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_pipepipe;
      tok->source.size = 2;
    }
    break;

  case '?':
    tok->kind = TOKEN_question;
    break;

  case ':':
    tok->kind = TOKEN_colon;
    break;

  case ';':
    tok->kind = TOKEN_semi;
    break;

  case '=':
    tok->kind = TOKEN_equal;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '=') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_equalequal;
      tok->source.size = 2;
    }
    break;

  case ',':
    tok->kind = TOKEN_comma;
    break;

  case '#':
    tok->kind = TOKEN_hash;
    if (SourceManagerCurrentChar(srcMgr).c[0] == '#') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_hashhash;
      tok->source.size = 2;
    } else if (SourceManagerCurrentChar(srcMgr).c[0] == '@') {
      SourceManagerGetChar(srcMgr);
      tok->kind = TOKEN_hashat;
      tok->source.size = 2;
    }
    break;

  default:
    PrintLexError("unexpected character", "unexpected character here.", tok);
    return false;
  }

  return true;
}

static bool MatchNumericConstant(struct SourceManager *srcMgr,
                                 struct Token *tok) {
  // TODO implement this
  return true;
}

static bool MatchCharConstant(struct SourceManager *srcMgr, struct Token *tok) {
  assert(SourceManagerCurrentChar(srcMgr).c[0] == '\'');
  SourceManagerGetChar(srcMgr);

  while (true) {
    Utf8Char c = SourceManagerCurrentChar(srcMgr);
    SourceManagerGetChar(srcMgr);
    if (c.c[0] == '\0' || c.c[0] == '\n') {
      tok->source.size = 1;
      PrintLexError(
          "unterminated char constant", "char constant starts here", tok);
      return false;
    }

    if (c.c[0] == '\'') {
      tok->kind = TOKEN_char_constant;
      tok->source.size = srcMgr->cursor - tok->source.data;
      return true;
    }

    if (c.c[0] == '\\') {
      SourceManagerGetChar(srcMgr);
    }
  }
}

static bool MatchWideCharConstant(struct SourceManager *srcMgr,
                                  struct Token *tok) {
  assert(SourceManagerCurrentChar(srcMgr).c[0] == 'L');
  SourceManagerGetChar(srcMgr);
  assert(SourceManagerCurrentChar(srcMgr).c[0] == '\'');
  return MatchCharConstant(srcMgr, tok);
}

static bool MatchStringLiteral(struct SourceManager *srcMgr,
                               struct Token *tok) {
  assert(SourceManagerCurrentChar(srcMgr).c[0] == '\"');
  SourceManagerGetChar(srcMgr);

  while (true) {
    Utf8Char c = SourceManagerCurrentChar(srcMgr);
    SourceManagerGetChar(srcMgr);
    if (c.c[0] == '\0' || c.c[0] == '\n') {
      tok->source.size = 1;
      PrintLexError(
          "unterminated string literal", "string starts here", tok);
      return false;
    }

    if (c.c[0] == '\"') {
      tok->kind = TOKEN_string_literal;
      tok->source.size = srcMgr->cursor - tok->source.data;
      return true;
    }

    if (c.c[0] == '\\') {
      SourceManagerGetChar(srcMgr);
    }
  }
}

static bool MatchWideStringLiteral(struct SourceManager *srcMgr,
                                   struct Token *tok) {
  assert(SourceManagerCurrentChar(srcMgr).c[0] == 'L');
  SourceManagerGetChar(srcMgr);
  assert(SourceManagerCurrentChar(srcMgr).c[0] == '\"');
  return MatchStringLiteral(srcMgr, tok);
}

static bool MatchIdentifierOrKeyword(struct SourceManager *srcMgr,
                                     struct Token *tok) {
  assert(IsIdentifierOrKeywordUtf8Char(SourceManagerCurrentChar(srcMgr)));
  while (IsIdentifierOrKeywordUtf8Char(SourceManagerCurrentChar(srcMgr)))
    SourceManagerGetChar(srcMgr);

  tok->source.size = srcMgr->cursor - tok->source.data;
  tok->kind = StringPieceToKeywordType(tok->source);
  if (tok->kind == TOKEN_unknown)
    tok->kind = TOKEN_identifier;
  return true;
}
