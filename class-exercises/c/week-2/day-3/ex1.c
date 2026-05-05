#include <stdio.h>





unsigned int byte_pack(unsigned char a, unsigned char b, unsigned char c,
                       unsigned char d);
int is_odd(unsigned char value);
int compare_nybble(unsigned char value);
void print_compare(int result);




int main(int argc, char **argv) {
  unsigned char value = 100;
  int result = is_odd(value);

  if (result) {
    printf("ODD\n");
  } else {
    printf("EVEN\n");
  }

  print_compare(compare_nybble(0x33));
  print_compare(compare_nybble(0xF2));
  print_compare(compare_nybble(0xC8));
  print_compare(compare_nybble(0xCF));

  printf("packed value: %X\n", byte_pack(0xCF, 0xC8, 0xF2, 0x33));
};

unsigned int byte_pack(unsigned char a, unsigned char b, unsigned char c,
                       unsigned char d) {

  unsigned int result = 0x0;
  // 00000000 00000000 00000000 00000000
  result = (a << 24) | (b << 16) | (c << 8) | d;

  return result;
}

int is_odd(unsigned char value) { return value & 1; }

int compare_nybble(unsigned char value) {
  /*
  low nybble == high nybble -> 0
  low nybble > high nybble -> 1
  low nybble < high nybble -> -1
  */

  unsigned char lowN = value & 0x0F;
  unsigned char highN = (value & 0xF0) >> 4;

  if (lowN == highN) {
    return 0;
  } else if (lowN > highN) {
    return 1;
  } else if (lowN < highN) {
    return -1;
  }

  return -2;
}

void print_compare(int result) {
  if (result == 0) {
    printf("low nybble is equal to high nybble\n");
  } else if (result == 1) {
    printf("low nybble is greater than high nybble\n");
  } else if (result == -1) {
    printf("low nybble is less than high nybble\n");
  }
}