#include <stdio.h>

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Usage: %s <character>\n", argv[0]);
    return 1;
  }

  unsigned char c = argv[1][0]; // 01110100

  printf("ASCII value of '%c': %d\n", c, c);

  unsigned char res = ((c & 0x70) | 0x02) ^ 0x20; //  0111 0100

  printf("ASCII value of '%c': %d\n", res, res);
  return 0;
}