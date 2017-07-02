#ifndef LNSD_H 
#define LNSD_H 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>

int lnsd(const char *src, const char *dest) {
  struct stat s;
  int r = lstat(src, &s);
  assert(r == 0);

  r = lstat(dest, &s);
  assert(r == -1 && errno == ENOENT);

  r = symlink(src, dest);
  assert(r == 0);

  return r;
}

#endif 
