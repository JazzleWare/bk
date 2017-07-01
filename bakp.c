#include <stdio.h>
#include "hash.h"
#include "fcompare.h"

#include "mdirs.h"

int main(int argc, char *argv[]) {
  char buf[1000] = {0};
  FILE *a = fopen(argv[1], ("rb"));
  hash(a, (u8 *)buf, 1000);
  printf("%s %s\n", buf, argv[1]);
  fseek(a, 0, SEEK_SET);
  FILE *b = fopen(argv[2], ("rb"));
  int l = fcomp(a,b);
  printf("%s %s %s\n", argv[1], argv[2], l ? "inequal" : "equal");
  fclose(a);
  fclose(b);
  printf("md [%s]\n", argv[3]);
  int m = md(argv[3]);
  printf("retv: [%i]\n", m);
  return 0;
}
