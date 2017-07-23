#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "errorf.h"
#include "mvon.h"
#include "lnsd.h"
#include "hash.h"
#include "mdirs.h"
#include "fcompare.h"
#include "list.h"
#include "join.h"

char *hashPath(const char *hash) {
  char head[] = { hash[0], hash[1], '/', '\0' };
  char *ht = join(head, hash+2);
  char *whole = join(".index", ht);
  free(ht);
  return whole;
}

void bk(struct iter *it, const char *npath, struct stat *s) { 
  struct bkreport *br = (struct bkreport *) it->ctx;
  br->total++; br->sz += s->st_size;
  if (S_ISDIR(s->st_mode)) {
    br-> ndirs++;
    printf("DIR [%s]\n", npath);
    iterPath(npath, it);
    return;
  }
  if (!S_ISREG(s->st_mode)) {
    if (S_ISLNK(s->st_mode)) {
      printf("LNK [%s]\n", npath );
      br-> lns++;
    } else
      printf("OTH [%s]\n", npath );
  return;
  }

  printf("REG [%s]\n", npath );

  FILE *a = fopen(npath, ("rb"));
  char hashBuf[1000];
  hash(a, (unsigned char *) hashBuf, 999);
  errorf("hash [%s]; len [%i]\n", hashBuf, strlen(hashBuf));

  assert(strlen(hashBuf) == 88);
  char *hashp = hashPath(hashBuf);
  char *payload = join(hashp, "payload");

  struct stat st;
  int r = lstat(hashp, &st);

  int isNewLn = 0;
  if (r == -1) {
    assert(errno == ENOENT);
    md(hashp);
    if (br->cp_if_new) {
      int payloadNameLen = strlen(payload);
      char *tail = ".temp";
      char *temp = (char *) malloc(payloadNameLen+strlen(tail)+1);
      memcpy(temp, payload, payloadNameLen);
      memcpy(&temp[payloadNameLen], tail, strlen(tail));
      temp[payloadNameLen+strlen(tail)] = '\0';
      cpsd(a, temp );
      mvon(temp, payload);
      free(temp );
      fclose(a);    
    }
    else {
      fclose(a);    
      mvon(npath, payload);
    }
    isNewLn = 1;
    br->reg_new.num++;
    br->reg_new.sz += s->st_size ;
  } else {
    assert(S_ISDIR(st.st_mode));
    FILE *b = fopen(payload, ("rb"));
    fseek(a, 0, SEEK_SET);
    assert(fcomp(a,b) == 0);
    fclose(a);
    fclose(b);
    if (!br->cp_if_new) unlink(npath);
    br->reg_existing.num++;
    br->reg_existing.sz += s->st_size ;
  }

  if (!br->cp_if_new) lnsd(payload, npath, isNewLn);

  char *t = join(hashp, "inv");
  FILE *inv = fopen(t, "a");
  free(t);

  br->reg_total.num++;
  br->reg_total.sz += s->st_size ;

  fprintf(inv, "\n[%s]", npath);
  fclose(inv);

  free(hashp);
  free(payload );
}

int main(int argc, char *argv[]) {
  struct bkreport br = {0};
  memset(&br, 0, sizeof(br));
  int i = 1;
  if (i >= argc) { printf("insufficient arguments.\n"); return 1; }
  const char *arg = argv[i];
  if (strcmp(arg, "--") == 0)
    i++;
  else if (strcmp(arg, "-c") == 0) {
    i++;
    br.cp_if_new = 1;
  }

  if (i >= argc) { printf("insuficient arguments.\n"); return 2; }

  struct iter it = { bk, &br };
  iterPath(argv[i], &it);


  printf("DIRS [%lld]\n", br.ndirs);
  printf("LNKS [%lld]\n", br.lns);
  printf("REGS [%lld] (%lld bytes)\n", br.reg_total.num, br.reg_total.sz );
  printf("  EXISTING [%lld] (%lld bytes)\n", br.reg_existing.num, br.reg_existing.sz );
  printf("  FRESH    [%lld] (%lld bytes)\n", br.reg_new.num, br.reg_new.sz );
  printf("------------------------------\n");
  printf("TOTAL [%lld] in [%lld] bytes\n", br.total, br.sz );

  return 0;
}
