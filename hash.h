#ifndef HASH_H
#define HASH_H
#include <stdio.h>
#include <openssl/sha.h>
#include <assert.h>

#include "itype.h"
#define BUF_LEN (512*512)

static char BASE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-+";

#define ins(buf, b) do { assert((buf) != BUF_LAST); *(buf)++ = (b); } while (0);
void hash(FILE *in, u8 *hashBuf, const len_t hashBufLen) {
  SHA512_CTX ctx;
  SHA512_Init(&ctx);

  u8 buf[BUF_LEN];

  while (1) {
    int len = fread(buf, 1, BUF_LEN, in);
    if (len <= 0)
      break;
    SHA512_Update(&ctx, buf, len);
  }

  unsigned char m[SHA512_DIGEST_LENGTH];
  SHA512_Final(m, &ctx);

  const size_t SHA512LEN = SHA512_DIGEST_LENGTH;

  u8 *out = hashBuf;
  const u8 *const BUF_LAST = hashBuf + hashBufLen;

  for (int i=0; i < SHA512LEN; i += 3) {
    unsigned char b = (m[i] & ~3) >> 2;
    ins(out, BASE[b]);

    b = (m[i] & 3) << 4;
    if (i+1 < SHA512LEN)
      b |= (m[i+1] & ~15) >> 4;
    ins(out, BASE[b]);

    if (i+1 < SHA512LEN) {
      b = (m[i+1] & 15) << 2;
      if (i+2 < SHA512LEN)
        b |= (m[i+2] & ~63) >> 6;
      ins(out, BASE[b]);
    } else { ins(out, '='); }

    if (i+2 < SHA512LEN) {
      b = m[i+2] & 63;
      ins(out, BASE[b]);
    } else { ins(out, '='); }
  }

  ins(out, '\0');
}
#endif 
