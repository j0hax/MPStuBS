// vim: set noet ts=4 sw=4:

#include "file_out.h"

int FileOut::counter = 0;

FileOut::FileOut(const char* path) {
  ++counter;
  fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
  int i = 0;

  for (const char* c = path; *c != '\0'; c++) {
    cpy_path[i++] = *c;
  }
}

FileOut::~FileOut() {
  --counter;
  close(fd);
}

const char* FileOut::getPath() {
  return cpy_path;
}

int FileOut::count() {
  return counter;
}

void FileOut::flush() {
  //printf("_%d\n", pos);
  //printf("%s\n",buffer);
  write(fd, (const void*)buffer, pos);
  pos = 0;
}
