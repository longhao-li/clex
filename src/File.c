#include "clex/File.h"
#include "clex/Error.h"
#include "clex/String.h"

#include <stdint.h>

static void SysFileError(const char *errMsg, const char *filename) {
  printf(PRINT_EMPHYSISE PRINT_COLOR_RED
         "error: " PRINT_COLOR_WHITE
         "%s: %s" PRINT_UNSET_EMPHYSISE PRINT_COLOR_DEFAULT "\n",
         errMsg,
         filename);
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
    close(fd);
    return NULL;
  }
  str->size = fileSize;
  close(fd);
  return str;
}

#else
#  include <Windows.h>

struct String *ReadFileToString(const char *path) {
  if (path == NULL)
    return NULL;

  OFSTRUCT fileInfo;

  HANDLE file = CreateFile(path,
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
  if (file == INVALID_HANDLE_VALUE) {
    SysFileError("failed to open file", path);
    return NULL;
  }

  LARGE_INTEGER fileSize;
  if (!GetFileSizeEx(file, &fileSize)) {
    SysFileError("failed to open file", path);
    return NULL;
  }

  struct String *str = CreateString(fileSize.QuadPart);
  DWORD readSize = 0;
  if (ReadFile(file, str->data, fileSize.QuadPart, &readSize, NULL) == 0) {
    SysFileError("failed to read file", path);
    DestroyString(&str);
    CloseHandle(file);
    return NULL;
  }

  CloseHandle(file);
  str->size = readSize;
  return str;
}

#endif
