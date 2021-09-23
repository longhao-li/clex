#ifndef CLEX_ERROR_H
#define CLEX_ERROR_H

#include "clex/Token.h"

#include <stddef.h>
#include <stdio.h>

void PrintLexError(const char *errSummary,
                   const char *errInfo,
                   const struct Token *tok);

static inline void SysError(const char *errMsg) {
  printf("\e[1m\033[31merror: \e[0m\033[0m%s\n", errMsg);
}

#endif // CLEX_ERROR_H
