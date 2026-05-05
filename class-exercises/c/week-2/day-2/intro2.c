#include <stdio.h>

int main() {
  unsigned byte = 0x7e;
  unsigned short word = 0x54f2;

  int byte_bit_count = 0;
  int word_bit_count = 0;
  unsigned char bmask = 1;
  unsigned short wmask = 1;

  for (int i = 0; i < 8; i++) {
    if (byte & bmask)
      byte_bit_count++;

    bmask = bmask << 1;
  }

  for (int i = 0; i < 16; i++) {
    if (word & wmask)
      word_bit_count++;

    wmask = wmask << 1;
  }

  printf("Byte bit count: %d\n", byte_bit_count);
  printf("Word bit count: %d\n", word_bit_count);
  return 0;
}