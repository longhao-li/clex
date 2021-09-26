#ifndef CLEX_ERROR_H
#define CLEX_ERROR_H

#include "clex/Token.h"

#include <stddef.h>
#include <stdio.h>

void PrintLexError(const char *errSummary,
                   const char *errInfo,
                   const struct Token *tok);

static inline void SysError(const char *errMsg) {
  printf("\e[1m\033[31merror: \033[1;37m%s\033[0m\e[0m\n", errMsg);
}

#endif // CLEX_ERROR_H
