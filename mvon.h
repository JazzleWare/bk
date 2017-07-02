#ifndef MVON_H
#define MVON_H
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <errno.h>
#include <assert.h>

int mvon(const char *o, const char *n) {
  struct stat s;
  int r = lstat(o, &s);
  assert(r == 0 && S_ISREG(s.st_mode));

  r = lstat(n, &s);
  assert(r == -1 && errno == ENOENT);

  r = rename(o, n);
  assert(r == 0);

  return r;
}

#endif              
