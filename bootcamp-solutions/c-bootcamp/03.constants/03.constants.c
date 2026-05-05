#include <stdio.h>

#define THREAD_NUM 2

const int PORT_NUMBER = 1023;
const int PROCESS_ID = 0;

int main(int argc, char **argv) {

  printf("%d\n", THREAD_NUM);
  printf("%d\n", PORT_NUMBER);
  printf("%d\n", PROCESS_ID);

  return 0;
}