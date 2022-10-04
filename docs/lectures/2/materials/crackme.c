#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct data { char name[64]; };
struct fp { int  (*fp)(); };

int winner() { return printf("You win\n"); }
int nowinner() { return printf("You lose\n"); }

int main(int argc, char *argv[]) {
  struct data *d;
  struct fp *f;

  d = malloc(sizeof(struct data));
  f = malloc(sizeof(struct fp));
  printf("data is at %p\nfp is at %p\n", d, f);

  f->fp = nowinner;
  strcpy(d->name, argv[1]);
  f->fp();

  return 0;
}
