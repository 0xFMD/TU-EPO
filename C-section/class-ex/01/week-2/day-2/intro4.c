#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: %s language 1 language 2\n", argv[0]);
    return 1;
  }

  printf("Language 1: %s\n", argv[1]);
  printf("Language 2: %s\n", argv[2]);

  return 0;
}
