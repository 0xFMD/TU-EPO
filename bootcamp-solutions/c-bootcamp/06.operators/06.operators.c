#include <stdio.h>

int main() {
  int a = 12; // Binary: 1100
  int b = 8;  // Binary: 1000

  printf("a & b = %d\n", a & b);   // output: 8 (Binary: 1000)
  printf("a | b = %d\n", a | b);   // output: 12 (Binary: 1100)
  printf("a ^ b = %d\n", a ^ b);   // output: 4 (Binary: 0100)
  printf("~a = %d\n", ~a);         // output: -13
  printf("a << 1 = %d\n", a << 1); // output: 24 (Binary: 11000)
  printf("a >> 1 = %d\n", a >> 1); // output: 6 (Binary: 110)
}
