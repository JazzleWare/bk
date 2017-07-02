#ifndef JOIN_H
#define JOIN_H
#include <string.h>

#include <stdlib.h>

char *join(const char *a, const char *b) {
  int oa = strlen(a) - 1;
  while (oa > 0 && a[oa] && a[oa] == '/') oa--;

  oa++;

  while (*b && *b == '/') b++;

  int len = oa + strlen(b) + 2;
  char *nPath = (char *) malloc(len);

  memcpy(nPath, a, oa);
  nPath[oa] = '/';
  memcpy(nPath+oa+1, b, strlen(b));
  nPath[oa+strlen(b)+1] = '\0';

  return nPath;
}
#endif 
