#include <stdio.h>

#define BEGIN_FOR(init, condition, increment) for (init; condition; increment) {
#define END_FOR }

#define BEGIN_DO do {
#define END_DO(expr) } while (expr);

int main(int argc, char **argv) {

  BEGIN_FOR(int i = 0, i < 10, i++)
  printf("%d ", i);
  END_FOR

        printf("\ndo while\n");

        int j = 0;
        BEGIN_DO
        printf("%d ", j);
        j++;
        END_DO(j < 10)
}