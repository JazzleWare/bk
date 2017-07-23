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

#define BUFLEN (512*512)
int cpsd(FILE *in, char *o, const char *linkContents) {
  FILE *out = fopen(o, "wb");
  if (in) fseek(in, 0, SEEK_SET);
  char b[BUFLEN];
  while (1) {
    int len = in ? fread(b, 1, BUFLEN, in) : strlen(linkContents);
    if (len <= 0 && linkContents == NULL) break;
    fwrite(in ? b : linkContents, 1, len, out);
    if (linkContents)
      break;
  }
  fclose(out);
  return 0;
}
#undef BUFLEN

#endif              
