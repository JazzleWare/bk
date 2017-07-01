#ifndef LIST_H
#define LIST_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include "join.h"

 typedef void (*iterFn)(void *ctx, const char *nPath);

struct iter {
  iterFn it;
  void *ctx;
};

void iterPath(const char *nPath, struct iter *it) {
  printf("iter [%s]\n", nPath);

  DIR *dirp = opendir(nPath);
  struct dirent *entry;
  struct stat s;
  while (entry = readdir(dirp)) {
    char *name = entry->d_name;

    printf("  entry [%s]\n", name);

    if ((strcmp(name, ".") == 0) || (strcmp(name, "..") == 0))
      continue;
    printf("head [%s]; entry [%s]\n", nPath, name );
    char *path = join(nPath, name);
    printf("strcmp(.) [%i]; strcmp(..) [%i]\n", strcmp(name, "."), strcmp(name, ".."));

    printf("stat [%s]", path);
    assert(lstat(path, &s) == 0);
    if (S_ISDIR(s.st_mode))
      iterPath(path, it);
    else if (S_ISREG(s.st_mode))
      it->it(it->ctx, path);

    free(path);
  }
  closedir(dirp);
}

#endif 
