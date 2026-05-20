#include <stdio.h>

#define swap(a, b)                                                             \
  {                                                                            \
    if (a > 0 & b > 0) {                                                       \
      int temp = a;                                                            \
      a = b;                                                                   \
      b = temp;                                                                \
    }                                                                          \
  }

int main() {
  int a = 3, b = 7;
  printf("Before swap: a = %d, b = %d\n", a, b);
  swap(a, b);
  printf("After swap: a = %d, b = %d\n", a, b);
  return 0;
}