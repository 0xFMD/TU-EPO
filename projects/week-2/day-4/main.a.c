
for (int y = 0; y < 10; y++) {
#ifdef OUTER_LOOP
  printf("outerloop: %d\n", y);
#ifdef INNER_LOOP
#include "main.b.c"
#endif
#endif
}

#ifdef INNER_LOOP
#ifndef OUTER_LOOP
int y = 0;
#include "main.b.c"
#endif
#endif
