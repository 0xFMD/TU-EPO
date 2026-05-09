
#include <stdio.h>

int main() {
  char var = 'A';
  char *pVar = &var;
  printf("\naddress of var: %p, value of var: %c\n", &var, var);
  printf("address stored in pVar: %p, value pointed to by pVar: %c\n", pVar,
         *pVar);

  *pVar = 'B';

  printf("\nAfter modifying value through pointer:\n\n");

  printf("address of var: %p, value of var: %c\n", &var, var);
  printf("address stored in pVar: %p, value pointed to by pVar: %c\n\n", pVar,
         *pVar);

  printf("\n\n incrementing pointer pVar +2 bytes:\n\n");

  *(pVar) += 2;

  printf("address stored in pVar: %p, value pointed to by pVar: %c\n\n", pVar,
         *pVar);
  return 0;
}