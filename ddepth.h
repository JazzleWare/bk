#ifndef UP_H
#define UP_H
#include <string.h>
#include <stdlib.h>
#include "join.h"

int   ddepth(const char *nPath) {
  int r = 0;
  const char *str = nPath;
  while (str = strchr(str, '/')) {
    r++;
    str++;
  }
  return r;
}

char *pathu(int len) {
  char *r = NULL;
  while (len--) {
    if (r == NULL) {
      r = (char *)malloc(3);
      r[0] = r[1] = '.';
      r[2] = '\0';
    }
    else {
      char *t = join(r,"..");
      free(r);
      r = t;
    }
  }

  return r;
}
#endif  
