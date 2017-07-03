#ifndef MDIRS_H 
#define MDIRS_H 

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "mdirs.h"
#include "itype.h"

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

int md(char *nPath) {
  char *str = strdup(nPath);
  char *tail = tailOf(str);

  char *head = NULL;
  if (tail == str)
    head = *tail != '/' ? "." : "/";
  else
    head = str;

  struct stat s;
  int r = lstat(head, &s);
  if (r == 0)
    errorf("head exists: [%s, %s]\n", head, S_ISDIR(s.st_mode) ? "directory" : "notdirectory");
  else {
    errorf("can't stat [%s]\n", head);
    assert(errno == ENOENT);
    errorf("making head: [%s]\n", head);
    md(head);
  }

  errorf("making tail: [%s]\n", tail);

  r = mkdir(nPath, 0777);

  free(str);

  if (r != 0)
    errorf("md [%s]\n", strerror(errno));

  return r;
}#endif 
