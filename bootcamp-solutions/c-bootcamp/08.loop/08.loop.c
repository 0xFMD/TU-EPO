
#include <stdio.h>

int main() {
  unsigned char letter = 'A';
  while (letter <= 'Z') {
    printf("%c%c ", letter++, (letter | 0x20));
  }
  return 0;
}
