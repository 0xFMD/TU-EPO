#include <stdio.h>

int main(int argc, char **argv) {

  int age;
  double height;
  float weight;
  char initial;

  age = 25;
  height = 174.5;
  weight = 55.6;
  initial = 'F';

  printf("Age:%d, type size:%zu\n", age, sizeof(age));
  printf("Height:%lf, type size:%zu\n", height, sizeof(height));
  printf("Weight:%f, type size:%zu\n", weight, sizeof(weight));
  printf("initial:%c, type size:%zu\n", initial, sizeof(initial));

  return 0;
}