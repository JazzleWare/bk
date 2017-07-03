#ifndef LNSD_H 
#define LNSD_H 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "ddepth.h"

int lnsd(const char *src, const char *dest, int isNewLn) {
  printf("lnsd [%s] [%s]\n", dest, src);
  struct stat s;
  int r = lstat(src, &s);
  if (r != 0)
    errorf("Error: [%s]", strerror(errno));

  fflush(stdout);
  assert(r == 0);

  r = lstat(dest, &s);

  if (isNewLn)
    assert(r == -1 && errno == ENOENT);

  int l = (int) ddepth(dest);
  assert(l > 0);

  char *u = (char *) pathu(l);
  char *t = join(u, src);
  r = symlink(t, dest);
  assert(r == 0);
  free(u);
  free(t);

  return r;
}

#endif 
