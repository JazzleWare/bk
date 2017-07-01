#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "mdirs.h"

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
    printf("head exists: [%s, %s]\n", head, S_ISDIR(s.st_mode) ? "directory" : "notdirectory");
  else {
    printf("can't stat [%s]\n", head);
    assert(errno == ENOENT);
    printf("making head: [%s]\n", head);
    md(head);
  }

  printf("making tail: [%s]\n", tail);

  r = mkdir(nPath, 0777);

  free(str);

  if (r != 0)
    printf("md [%s]\n", strerror(errno));

  return r;
}
