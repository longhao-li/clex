#ifndef CLEX_LEXER_H
#define CLEX_LEXER_H

#include "clex/Bool.h"
#include "clex/SourceManager.h"
#include "clex/Token.h"

bool GetToken(struct SourceManager *srcMgr, struct Token *tok);

#endif // CLEX_LEXER_H
