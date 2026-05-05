/* #include <stdio.h> */

#define mbyte unsigned char;
typedef unsigned char tbyte;

#define BYTE_SHL(value, n_shift) ((value) << (n_shift))
#define BYTE_SHR(value, n_shift) ((value) >> (n_shift))

#define BYTE_SWAP_NYBBLE(value) (BYTE_SHL((value), 4) | BYTE_SHR((value), 4))

int main(int argc, char **argv) {
  tbyte v = 0x2E;
  v = BYTE_SWAP_NYBBLE(v);

  /* printf("%X\n", v); */
}