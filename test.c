#include <stdio.h>
#include <stdlib.h>

#define HAS_ERRORF
#include "errorf.h"

#include "hash.h"
#include "lnsd.h"
#include "fcompare.h"

#include "mdirs.h"
#include "mvon.h"
#include "list.h"
#include "ddepth.h"

void iterFun(struct iter *it, const char *nPath, struct stat *s) {
  struct bkreport *br = (struct bkreport *) it->ctx;
  br->total++; br->sz += s->st_size;
  if (S_ISDIR(s->st_mode)) {
    br-> ndirs++;
    printf("iter [%i]", br-> ndirs );
    iterPath(nPath, it);
  }
  else {
    if (!S_ISREG(s->st_mode)) {
      if (S_ISLNK(s->st_mode))
        br-> lns++;
    } else {
      br->reg_total.num++;
      br->reg_total.sz += s->st_size ;
    }
    printf("[%s]\n", nPath);
  }
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
    return lnsd(argv[2], argv[3], 1);

  if (strcmp(argv[1], "-c") == 0) {
    FILE
      *a = fopen(argv[2], ("rb")),
      *b = fopen(argv[3], ("rb"));
    int r = fcomp(a,b,NULL);
    fclose(a);
    fclose(b);

    printf("%s %s %s\n", argv[2], argv[3], r ? "inequal" : "equal");
    return r;
  }

  if (strcmp(argv[1], ("-p")) == 0) {
    printf("path [%s]\n", argv[2]);
    int r = ddepth(argv[2]);
    char *u = pathu(r);
    printf("l [%i] [%s]\n", r, u);
    free(u);

    return md(argv[2]);
  }

  if (strcmp(argv[1], "-i") == 0) {
    struct bkreport br;
    memset(&br, 0, sizeof(br));
    struct iter it = { iterFun, (void *) &br };
    iterPath(argv[2], &it);

    printf("DIRS [%lld]\n", br.ndirs);
    printf("LNKS [%lld]\n", br.lns);
    printf("REGS [%lld] (%lld bytes)\n", br.reg_total.num, br.reg_total.sz );
    printf("  EXISTING [%lld] (%lld bytes)\n", br.reg_existing.num, br.reg_existing.sz );
    printf("  FRESH    [%lld] (%lld bytes)\n", br.reg_new.num, br.reg_new.sz );
    printf("------------------------------\n");
    printf("TOTAL [%lld] in [%lld] bytes\n", br.total, br.sz );

    return 0;
  }

  if (strcmp(argv[1], "-h") == 0) {
    char buf[1000] = {0};
    FILE *a = fopen(argv[2], ("rb"));
    hash(a, (u8 *)buf, 1000, NULL);
    printf("%s %s\n", buf, argv[1]);
    fclose(a);
    return 0;
  }

  printf("invalid [%s]\n", argv[1]);
  return -1;
}
