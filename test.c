#include <stdio.h>
#include "hash.h"
#include "fcompare.h"

#include "mdirs.h"
#include "lnsd.h"
#include "list.h"
#include "mvon.h"

void iterFun(void *ctx, const char *nPath) {
  ++*(long long *) ctx;
  printf("got [%s]\n", nPath );
}

// -m [from] [to] -> mvon(from, to)
// -l [from] [to] -> lnsd(from, to)
// -c [a] [b] -> fcomp(a,b)
// -p [name] -> md(name)

// -i [path] -> iter(path)
// -h [file] -> hash(file)

int main(int argc, char *argv[]) {
  if (strcmp(argv[1], "-m") == 0)
    return mvon(argv[2], argv[3]);

  if (strcmp(argv[1], "-l") == 0)
    return lnsd(argv[2], argv[3]);

  if (strcmp(argv[1], "-c") == 0) {
    FILE
      *a = fopen(argv[2], ("rb")),
      *b = fopen(argv[3], ("rb"));
    int r = fcomp(a,b);
    fclose(a);
    fclose(b);

    printf("%s %s %s\n", argv[2], argv[3], r ? "inequal" : "equal");
    return r;
  }

  if (strcmp(argv[1], ("-p")) == 0) { return md(argv[2]); }

  if (strcmp(argv[1], "-i") == 0) {
    long long n = 0;
    struct iter it = { iterFun, (void *) &n };
    iterPath(argv[2], &it);
    printf("num [%lld]\n", n);
    return 0;
  }

  if (strcmp(argv[1], "-h") == 0) {
    char buf[1000] = {0};
    FILE *a = fopen(argv[2], ("rb"));
    hash(a, (u8 *)buf, 1000);
    printf("%s %s\n", buf, argv[1]);
    fclose(a);
    return 0;
  }

  printf("invalid [%s]\n", argv[1]);
  return -1;
}
