#include "clex/Error.h"
#include "clex/File.h"

#include <stdio.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    SysError("Expect at least one input file.");
    return 0;
  }

  struct String *source = ReadFileToString(argv[1]);
  if (source) {
    printf("%s\n", source->data);

    struct Token tok;
    tok.source = (struct StringPiece){source->data + 10, 10};
    tok.startOfLine = source->data;
    tok.row = 1;
    tok.col = 11;
    tok.kind = TOKEN_unknown;

    PrintLexError("Unknown error", "This is error info", &tok);
  }
  return 0;
}
