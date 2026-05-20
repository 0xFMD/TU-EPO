#include <stdio.h>

extern int VALUE;

void count_calls(void);
int sum_to(int n);

int main() {
  auto int x = 5;
  printf("auto x = %d\n", x);

  printf("Sum from 1 to 10 = %d\n", sum_to(10));

  count_calls();
  count_calls();
  count_calls();

  printf("shared = %d\n", VALUE);

  return 0;
}