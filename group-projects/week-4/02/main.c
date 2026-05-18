#include <stdio.h>
#include <stdlib.h>

#define INT_TYPE 0
#define FLOAT_TYPE 1
#define CHAR_TYPE 2
#define DOUBLE_TYPE 3

typedef struct Node {
  void *data;
  int size;
  unsigned int bitFields : 3;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct List {
  Node *head;
  Node *tail;
  int count;
} List;

Node *create_node(void *data, int type, int size) {

  Node *new_node = malloc(sizeof(Node));
  new_node->data = data;
  new_node->size = size;
  new_node->prev = NULL;
  new_node->next = NULL;

  switch (type) {
  case INT_TYPE:
    new_node->bitFields = INT_TYPE;
    break;
  case FLOAT_TYPE:
    new_node->bitFields = FLOAT_TYPE;
    break;
  case CHAR_TYPE:
    new_node->bitFields = CHAR_TYPE;
    break;
  default:
    printf("\n\nType error!\n\n");
    exit(0);
  }

  return new_node;
}

void insert_first(List *lst, Node *new_node) {

  if (lst->head == NULL) {
    lst->head = new_node;
    lst->tail = new_node;
    lst->count++;
    return;
  }

  new_node->next = lst->head;
  lst->head->prev = new_node;
  lst->head = new_node;
  lst->count++;
}

void insert_last(List *lst, Node *new_node) {

  if (lst->head == NULL) {
    lst->head = new_node;
    lst->tail = new_node;
    lst->count++;
    return;
  }

  new_node->prev = lst->tail;
  lst->tail->next = new_node;
  lst->tail = new_node;
  lst->count++;
}

void delete_first(List *lst) {
  if (lst->head == NULL)
    return;
  Node *the_one = lst->head;
  lst->head = lst->head->next;
  if (lst->head == NULL)
    lst->tail = NULL;
  else
    lst->head->prev = NULL;
  lst->count--;
  free(the_one->data);
  free(the_one);
}

void delete_last(List *lst) {
  if (lst->tail == NULL)
    return;

  Node *the_one = lst->tail;
  lst->tail = lst->tail->prev;
  // if the list will be empty after deletion
  if (lst->tail == NULL)
    lst->head = NULL;
  else
    lst->tail->next = NULL;

  lst->count--;
  free(the_one->data);
  free(the_one);
}

void print_list(List *lst) {

  printf("\n---list info---\n");
  printf("list count : %d\n", lst->count);

  Node *current = lst->head;
  while (current != NULL) {
    printf("--- current node ---\n");
    printf("size : %d\n", current->size);
    printf("type : %d\n", current->bitFields);
    printf("data : ");

    switch (current->bitFields) {
    case INT_TYPE:
      printf("%d\n", *(int *)current->data);
      break;
    case FLOAT_TYPE:
      printf("%.2f\n", *(float *)current->data);
      break;
    case CHAR_TYPE:
      printf("%c\n", *(char *)current->data);
      break;

    case DOUBLE_TYPE:
      printf("%.2lf\n", *(double *)current->data);
      break;
    }

    current = current->next;
  }
}

void free_list(List *lst) {
  Node *current = lst->head;
  while (current != NULL) {
    Node *temp = current;
    current = current->next;
    free(temp->data);
    free(temp);
  }
  lst->head = NULL;
  lst->tail = NULL;
  lst->count = 0;
}

int main() {
  List *lst = malloc(sizeof(List));
  lst->head = NULL;
  lst->tail = NULL;
  lst->count = 0;

  int *n1 = malloc(sizeof(int));
  *n1 = 111;
  char *n2 = malloc(sizeof(char));
  *n2 = 'A';
  float *n3 = malloc(sizeof(float));
  *n3 = 222;
  int *n4 = malloc(sizeof(int));
  *n4 = 333;

  Node *node = create_node(n1, INT_TYPE, sizeof(int));
  insert_first(lst, node);

  Node *node2 = create_node(n2, CHAR_TYPE, sizeof(char));
  insert_last(lst, node2);

  Node *node3 = create_node(n3, FLOAT_TYPE, sizeof(float));
  insert_last(lst, node3);

  Node *node4 = create_node(n4, INT_TYPE, sizeof(int));
  insert_first(lst, node4);

  delete_first(lst);
  delete_last(lst);

  print_list(lst);

  free_list(lst);
  free(lst);
  return 0;
}