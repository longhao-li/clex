#ifndef CLEX_ERROR_H
#define CLEX_ERROR_H

#include "clex/Token.h"

#include <stddef.h>
#include <stdio.h>

#if defined(__linux__) || defined(unix) || defined(__APPLE__)
#  define PRINT_EMPHYSISE "\e[1m"
#  define PRINT_UNSET_EMPHYSISE "\e[0m"
#  define PRINT_COLOR_RED "\033[31m"
#  define PRINT_COLOR_WHITE "\033[1;37m"
#  define PRINT_COLOR_DEFAULT "\033[0m"
#else
#  define PRINT_EMPHYSISE
#  define PRINT_UNSET_EMPHYSISE
#  define PRINT_COLOR_RED "\033[31m"
#  define PRINT_COLOR_GREEN "\033[32m"
#  define PRINT_COLOR_WHITE "\033[1;37m"
#  define PRINT_COLOR_DEFAULT "\033[0m"
#endif

void PrintLexError(const char *errSummary,
                   const char *errInfo,
                   const struct Token *tok);

static inline void SysError(const char *errMsg) {
  printf(PRINT_EMPHYSISE PRINT_COLOR_RED
         "error: " PRINT_COLOR_WHITE
         "%s" PRINT_UNSET_EMPHYSISE PRINT_COLOR_DEFAULT "\n",
         errMsg);
}

#endif // CLEX_ERROR_H
