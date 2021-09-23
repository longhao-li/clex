#include "clex/Error.h"

void PrintLexError(const char *errSummary,
                   const char *errInfo,
                   const struct Token *tok) {
  printf("\e[1m"           // bold
         "\033[1;37m"      // white
         "%zu:%zu: "       // line:column
         "\033[31m"        // red
         "error: "         // error:
         "\033[1;37m"      // white
         "%s"              // errSummary
         "\033[0m\e[0m\n", // default
         tok->row,
         tok->col,
         errSummary);
  const char *srcLineCur = tok->startOfLine;
  while (srcLineCur[0] != '\0' && srcLineCur[0] != '\n') {
    putchar(srcLineCur[0]);
    srcLineCur += 1;
  }
  putchar('\n');

  for (size_t i = 0; i < tok->source.data - tok->startOfLine; i++)
    putchar(' ');
  // bold, color
  printf("\e[1m\033[32m");
  for (size_t i = 0; i < tok->source.size; i++)
    putchar('^');
  printf("%s\n", errInfo);
  printf("\e[0m\033[0m");
}
