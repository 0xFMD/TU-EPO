#include <stdio.h>

int VALUE = 100;

void count_calls(void) {
  static int count = 0;
  count++;
  printf("\"count_calls\" function called: %d\n", count);
}

int sum_to(int n) {
  register int sum = 0;
  for (register int i = 1; i <= n; i++)
    sum += i;
  return sum;
}