#include <stdio.h>
#include <openssl/sha.h>
char buf[1000] = {0};

char BASE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-+";

int main(int argc, char *argv[]) {
  const char *const name = argv[1];

  SHA512_CTX ctx;
  SHA512_Init(&ctx);

  const size_t BUF_LEN = sizeof buf;
  FILE *in = fopen(name, ("rb"));

  while (1) {
    int len = fread(buf, 1, BUF_LEN, in);
    if (len <= 0)
      break;
    SHA512_Update(&ctx, buf, len);
  }

  fclose(in);

  unsigned char m[SHA512_DIGEST_LENGTH];
  SHA512_Final(m, &ctx);

  const size_t SHA512LEN = SHA512_DIGEST_LENGTH;

  for (int i=0; i < SHA512LEN; i += 3) {
    unsigned char b = (m[i] & ~3) >> 2;
    printf("%c", BASE[b]);

    b = (m[i] & 3) << 4;
    if (i+1 < SHA512LEN)
      b |= (m[i+1] & ~15) >> 4;
    printf("%c", BASE[b]);

    if (i+1 < SHA512LEN) {
      b = (m[i+1] & 15) << 2;
      if (i+2 < SHA512LEN)
        b |= (m[i+2] & ~63) >> 6;
      printf("%c", BASE[b]);
    } else { printf("="); }

    if (i+2 < SHA512LEN) {
      b = m[i+2] & 63;
      printf("%c", BASE[b]);
    } else { printf("="); }
  }

  printf("\n");

  return 0;
}
