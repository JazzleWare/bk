#ifndef MDIRS_H 
#define MDIRS_H 
#include <string.h>
#include "itype.h"
int  md(char *nPath);
static char *tailOf(char *nPath) {
  char *r = strrchr(nPath, '/');
  if (r == nPath)
    return r;

  if (r && *(r+1)) {
    *r = 0;
    return r+1;
  }
  return nPath;
}

#endif 
