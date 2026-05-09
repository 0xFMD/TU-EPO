
#include <stdio.h>

float calculateArea(float length, float width);

int main() {
  float l = 3, w = 5;
  printf("Rectangle area: %.2f\n", calculateArea(l, w));
  return 0;
}

float calculateArea(float length, float width) { return length * width; }