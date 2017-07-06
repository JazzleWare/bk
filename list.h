#ifndef LIST_H
#define LIST_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include "join.h"

struct iter;
 typedef void (*iterFn)(struct iter *iter, const char *nPath, struct stat *s);

struct iter {
  iterFn it;
  void *ctx;
};

void iterPath(const char *nPath, struct iter *it) {
  DIR *dirp = opendir(nPath);
  struct dirent *entry;
  struct stat s;
  while (entry = readdir(dirp)) {
    char *name = entry->d_name;
    if ((strcmp(name, ".") == 0) || (strcmp(name, "..") == 0))
      continue;
    char *path = join(nPath, name);

    assert(lstat(path, &s) == 0);
    if (S_ISDIR(s.st_mode))
      iterPath(path, it);
    else if (S_ISREG(s.st_mode)) {
      if (S_ISLNK(s.st_mode))
        errorf("---------------------------------[%s]---------------------------", nPath );
      it->it(it->ctx, path);
    }

    free(path);
  }
  closedir(dirp);
}

#endif 
