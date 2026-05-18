#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT_STUDENT(s)                                                       \
                                                                               \
  printf("  id: %d | name: %s | age: %d\n", s->id, s->name, s->age);           \
  if (s->prev)                                                                 \
    printf("  prev -> id: %d | name: %s | age: %d\n", s->prev->id,             \
           s->prev->name, s->prev->age);                                       \
  else                                                                         \
    printf("  prev -> NULL\n");                                                \
  if (s->next)                                                                 \
    printf("  next -> id: %d | name: %s | age: %d\n", s->next->id,             \
           s->next->name, s->next->age);                                       \
  else                                                                         \
    printf("  next -> NULL\n");                                                \
  printf("\n");

typedef struct Student {
  int id;
  char *name;
  int age;
  struct Student *next;
  struct Student *prev;
} Student;

Student *create_student(char *name, int age) {
  static int id = 1;

  Student *new_student = malloc(sizeof(Student));
  new_student->name = malloc(strlen(name) + 1);

  strcpy(new_student->name, name);
  new_student->id = id++;
  new_student->age = age;
  new_student->prev = NULL;
  new_student->next = NULL;

  return new_student;
}

int is_exists(Student *head, int id) {
  if (!head)
    return 0;

  Student *current = head;

  while (current != NULL) {
    if (current->id == id)
      return 1;

    current = current->next;
  }

  return 0;
}




Student *insert_first(Student *head, Student *new_student) {
  if (!new_student)
    return head;

  if (!head)
    return new_student;

  if (is_exists(head, new_student->id)) {
    free(new_student->name);
    free(new_student);
    return head;
  }
  new_student->next = head;
  head->prev = new_student;
  return new_student;
}




Student *insert_last(Student *head, Student *new_student) {
  if (!new_student)
    return head;

  if (!head)
    return new_student;

  if (is_exists(head, new_student->id)) {
    free(new_student->name);
    free(new_student);
    return head;
  }

  Student *current = head;

  while (current->next != NULL)
    current = current->next;

  new_student->prev = current;
  current->next = new_student;
  return head;
}



void insert_after(Student *head, int id, Student *new_student) {
  
  if (!head || is_exists(head, new_student->id)) {
    free(new_student->name);
    free(new_student);
    return;
  }

  Student *current = head;
  while (current != NULL) {

    if (current->id == id) {
      new_student->prev = current;
      new_student->next = current->next;

      // if not last node
      if (current->next != NULL)
        current->next->prev = new_student;

      current->next = new_student;
      return;
    }
    current = current->next;
  }
}





Student *search_student(Student *head, int id) {
  if (!head)
    return NULL;

  Student *current = head;

  while (current != NULL) {
    if (current->id == id) {
      printf("\n\nStudent - %d Found\n\n", id);
      PRINT_STUDENT(current);
      return current;
    }
    current = current->next;
  }

  return NULL;
}

void update_student(Student *head, int id, char *name, int age) {
  if (!head || !name)
    return;
    
  Student *the_one = search_student(head, id);
  if (!the_one)
    return;

  free(the_one->name);
  the_one->name = malloc(strlen(name) + 1);
  strcpy(the_one->name, name);
  the_one->age = age;

  printf("\n\nstudent with id %d, updated\n\n", id);
  PRINT_STUDENT(the_one);
}



Student *delete_student(Student *head, int id) {
  if (!head)
    return NULL;

  Student *current = head;
  Student *the_one = NULL;

  while (current != NULL) {
    if (current->id == id) {
      the_one = current;

      if (head == current)
        head = current->next;
      else
        current->prev->next = current->next;

      // if not last node
      if (current->next != NULL)
        current->next->prev = current->prev;

      break;
    }
    current = current->next;
  }

  if (!the_one)
    return head;

  printf("\n\nstudent with id %d, deleted\n\n", id);
  PRINT_STUDENT(the_one);
  free(the_one->name);
  free(the_one);
  return head;
}



void print_list(Student *head) {
  Student *current = head;
  while (current != NULL) {
    printf("--- current student ---\n");
    PRINT_STUDENT(current);
    current = current->next;
  }
  
}



Student *free_student(Student *head) {
  Student *current = head;

  while (current != NULL) {
    Student *temp = current;
    current = current->next;
    free(temp->name);
    free(temp);
  }

  return NULL;
}



int main() {
  Student *lst = NULL;
  lst = insert_first(lst, create_student("Student-1", 26));
  lst = insert_last(lst, create_student("Student-2", 26));
  lst = insert_last(lst, create_student("Student-3", 26));
  lst = insert_first(lst, create_student("Student-4", 26));

  update_student(lst, 2, "updated-Student-1", 16);

  insert_after(lst, 1, create_student("Inserted-Student-5", 26));

  lst = delete_student(lst, 3);

  print_list(lst);
  lst = free_student(lst);
  return 0;
}