#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "mvon.h"
#include "lnsd.h"
#include "hash.h"
#include "mdirs.h"
#include "fcompare.h"
#include "list.h"
#include "join.h"

char *hashPath(hash) {
  char head[] = { hash[0], hash[1], '/', '\0' };
  char *ht = join(head, hash+2);
  char *whole = join(".index", ht);
  free(ht);
  return whole;
}

void bk(void *ctx, const char *npath) {
  FILE *a = fopen(npath, ("rb"));
  char hash[1000];
  hash(a, (unsigned char *) hash, 999);
  assert(strlen(hash) == 88);
  char *hashp = hashPath(hash);
  char *payload = join(hashp, "payload");

  struct stat s;
  int r = lstat(npath, &s);

  if (r == -1) {
    assert(errno == ENOENT);
    md(hashp);
    fclose(a);
    mvon(npath, payload);
  } else {
    assert(S_ISDIR(s.st_mode);
    FILE *b = fopen(payload, ("rb"));
    fseek(a, 0, SEEK_SET);
    assert(fcomp(a,b) == 0);
    fclose(a);
    fclose(b);
  }

  lnsd(payload, npath);

  free(hashp); free(payload );
}

int main(int argc, char *argv[]) {
  struct iter it = { bk, NULL };
  iterPath(argv[1], &it);
  return 0;
}
