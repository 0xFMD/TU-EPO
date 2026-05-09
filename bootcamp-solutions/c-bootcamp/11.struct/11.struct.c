#include <stdio.h>

typedef struct Employee {
  int id;
  char name[50];
  float salary;
} Employee;

int main() {
  Employee x = {.id = 1, .name = "FMD", .salary = 999.99};

  printf("ID: %d\n", x.id);
  printf("Name: %s\n", x.name);
  printf("Salary: %.2f\n", x.salary);
}