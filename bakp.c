#include <stdio.h>
#include "hash.h"
int main(int argc, char *argv[]) {
  char buf[1000] = {0};
  FILE *in = fopen(argv[1], ("rb"));
  hash(in, (u8 *)buf, 1000);
  fclose(in);
  printf("%s %s", buf, argv[1]);
  return 0;
}
