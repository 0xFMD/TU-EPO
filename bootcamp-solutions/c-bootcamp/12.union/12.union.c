#include <stdio.h>

typedef union Value {
  int id;
  float price;
  char description[30];
} Value;

int main() {
  Value x = {.id = 1};

  printf("ID: %d\n", x.id);

  x.price = 999.99;
  printf("price: %.2f\n", x.price);

  printf("ID (overwrote): %d\n", x.id);

  return 0;
}