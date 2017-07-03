#ifndef FCOMPARE_H
#define FCOMPARE_H
#include <stdio.h>
#include "itype.h"
#include <string.h>

#define BUF_LEN (512*512)

int fcomp(FILE *a, FILE *b) {
  u8 buf_a[BUF_LEN], buf_b[BUF_LEN];
  len_t len_a = 0, len_b = 0;
  while (1) {
    len_a = fread(buf_a, 1, BUF_LEN, a);
    len_b = fread(buf_b, 1, BUF_LEN, b);
    if (len_a != len_b)
      return len_a - len_b;
    if (len_a == 0)
      break;
    int l = memcmp(buf_a, buf_b, len_a);
    if (l)
      return l;
  }
  return 0;
}
#endif 
