#include <stdio.h>

int main() {

  unsigned int dword = 0x4D6A3FE2;

  /* unsigned short low_word = dword & 0x0000FFFF;
  dword = (dword >> 16) | ((low_word) << 16);

  printf("%x", dword); */

  for (int i = 0; i < 16; i++) {
    unsigned int lsb = dword;
    dword = (dword >> 1) | (lsb << 31);
  }
  printf("%x", dword);

  return 0;
}