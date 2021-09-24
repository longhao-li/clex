#include "clex/Error.h"
#include "clex/File.h"
#include "clex/Lexer.h"
#include "clex/TokenKinds.h"

#include <stdio.h>

static inline void PrintToken(const struct Token *tok) {
  printf("[TokenKind: %s, Source: ", GetTokenName(tok->kind));
  PrintStringPiece(tok->source);
  printf("]\n");
}

int main(int argc, char **argv) {
  if (argc < 2) {
    SysError("Expect at least one input file.");
    return 0;
  }

  struct String *source = ReadFileToString(argv[1]);
  if (source) {
    struct Token tok;
    struct SourceManager *srcMgr =
        CreateSourceManager(StringPieceFromString(source));

    while (true) {
      bool res = GetToken(srcMgr, &tok);
      if (!res)
        continue;
      if (tok.kind == TOKEN_eof)
        break;
      PrintToken(&tok);
    }
  }
  return 0;
}
