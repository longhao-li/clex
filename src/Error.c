#include "clex/Error.h"

extern const char *CurrentFileName;

void PrintLexError(const char *errSummary,
                   const char *errInfo,
                   const struct Token *tok) {
  printf(PRINT_EMPHYSISE                            // bold
             PRINT_COLOR_WHITE                      // white
         "%s:%zu:%zu: "                             // filename:line:column
         PRINT_COLOR_RED                            // red
         "error: "                                  // error:
         PRINT_COLOR_WHITE                          // white
         "%s\n"                                     // errSummary
         PRINT_UNSET_EMPHYSISE PRINT_COLOR_DEFAULT, // default
         CurrentFileName,
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
  printf(PRINT_EMPHYSISE PRINT_COLOR_GREEN);
  for (size_t i = 0; i < tok->source.size; i++)
    putchar('^');
  printf("%s\n", errInfo);
  printf(PRINT_UNSET_EMPHYSISE PRINT_COLOR_DEFAULT);
}
