#include "clex/File.h"
#include "clex/Error.h"
#include "clex/String.h"
#include <stdint.h>

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
    SysError("Failed to open file.");
    return NULL;
  }
  size_t fileSize = buf.st_size;

  // failed to open file
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    SysError("Failed to open file.");
    return NULL;
  }

  struct String *str = CreateString(fileSize);
  int64_t readSize = read(fd, str->data, fileSize);

  if (readSize < 0) {
    SysError("Failed to read file.");
    DestroyString(&str);
    return NULL;
  }
  str->size = fileSize;
  return str;
}

#else
#  error "Unix environment required."
#endif
