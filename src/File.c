#include "clex/File.h"
#include "clex/Error.h"
#include "clex/String.h"
#include <stdint.h>

static void SysFileError(const char *errMsg, const char *filename) {
  printf(
      "\e[1m\033[31merror: \033[1;37m%s: %s\033[0m\e[0m\n", errMsg, filename);
}

#if defined(__linux__) || defined(unix) || defined(__APPLE__)
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <unistd.h>

struct String *ReadFileToString(const char *path) {
  if (path == NULL)
    return NULL;

  // Get file size
  struct stat buf;
  if (stat(path, &buf) < 0) {
    SysFileError("failed to open file", path);
    return NULL;
  }
  size_t fileSize = buf.st_size;

  // failed to open file
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    SysFileError("failed to open file", path);
    return NULL;
  }

  struct String *str = CreateString(fileSize);
  int64_t readSize = read(fd, str->data, fileSize);

  if (readSize < 0) {
    SysFileError("failed to read file", path);
    DestroyString(&str);
    return NULL;
  }
  str->size = fileSize;
  return str;
}

#else
#  error "Unix environment required."
#endif
