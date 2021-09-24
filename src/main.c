#include "clex/Error.h"
#include "clex/File.h"
#include "clex/Lexer.h"
#include "clex/Statistics.h"
#include "clex/TokenKinds.h"

#include <stdio.h>

const char *CurrentFileName = NULL;

static inline void PrintToken(const struct Token *tok) {
  printf("[TokenKind: %s, Source: ", GetTokenName(tok->kind));
  PrintStringPiece(tok->source);
  printf("]\n");
}

static inline void ParseFile(const char *filename) {
  CurrentFileName = filename;
  struct String *source = ReadFileToString(filename);

  if (source) {
    ClearStatistics();

    struct Token tok;
    struct SourceManager *srcMgr =
        CreateSourceManager(StringPieceFromString(source));

    while (true) {
      bool res = GetToken(srcMgr, &tok);
      if (!res)
        continue;

      if (tok.kind == TOKEN_eof)
        break;

      AddTokenToStatistics(&tok);
      PrintToken(&tok);
    }

    PrintStatistics();
    printf("Total lines: %zu\n"
           "Number of characters: %zu\n",
           srcMgr->row,
           srcMgr->numOfChar);

    DestroySourceManager(&srcMgr);
    DestroyString(&source);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    SysError("Expect at least one input file.");
    return 0;
  }

  for (int i = 1; i < argc; i++)
    ParseFile(argv[i]);

  return 0;
}
