#include <stdio.h>

int main() {
  const int value = 10;

  if (value % 2 == 0) {
    printf("The value is even.\n");
  } else {
    printf("The value is odd.\n");
  }

  return 0;
}