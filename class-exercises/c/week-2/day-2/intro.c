#include <stdio.h>

int main() {
  /* byte = 0x4e
   dword =0x043cafe0 */
  unsigned byte = 0x4e;
  unsigned short word = 0x3cfa;
  unsigned int dword = 0x00000000;

  /* output should be 0x043cafe0 */
  dword = dword | (byte << 20);
  dword = dword | (word << 8);
  dword = dword & 0x0F3FAFFF;
  dword = dword | 0x00000FE0;

  printf("%x", dword);
  return 0;
}