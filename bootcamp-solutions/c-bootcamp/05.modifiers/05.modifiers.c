#include <stdio.h>

int main() {
  unsigned char u_c = 1;
  signed char s_c = -1;
  short int s_i = 2;
  long double l_d = 3.0;
  unsigned long int ll_i = 5;

  printf("unsigned char - value: %u, size: %zu bytes\n", u_c, sizeof(u_c));
  printf("signed char - value: %d, size: %zu bytes\n", s_c, sizeof(s_c));
  printf("short int - value: %d, size: %zu bytes\n", s_i, sizeof(s_i));
  printf("long double - value: %.2Lf, size: %zu bytes\n", l_d, sizeof(l_d));
  printf("unsigned long int - value: %lu, size: %zu bytes\n", ll_i, sizeof(ll_i));

  return 0;
}