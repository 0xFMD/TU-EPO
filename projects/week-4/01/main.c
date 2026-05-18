#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERS_NUM 25

typedef struct User {
  int id;
  char *username;
  struct UserNode *following;
  struct UserNode *followers;
  int following_count;
  int followers_count;
} User;


typedef struct UserNode {
  struct User *user;
  struct UserNode *next;
} UserNode;





User *create_user(char *username);
void seed_users(User **users, bool is_with_followers);
void follow(User *a, User *b);
void unfollow(User *a, User *b);
User *find_user(User **users, char *username);
bool remove_from_list(UserNode **list, User *user);
bool is_following(User *a, User *b);
void print_user_info(User *user);
void print_users(User **users);



int main() {
  srand(time(NULL));
  User **users = malloc(sizeof(User *) * USERS_NUM);

  do {
    
    printf("seed users with random followers? (y/n): ");
    
    char choice;
    scanf(" %c", &choice);
    choice = tolower(choice);
    
    switch (choice) {
    case 'y':
      seed_users(users, true);
      print_users(users);
      break;

    case 'n':
      seed_users(users, false);
      follow(users[0], users[1]);
      follow(users[0], users[4]);
      follow(users[1], users[2]);
      print_users(users);

      unfollow(users[0], users[1]);
      printf("\n\n--- %s unfollowed %s ----\n", users[0]->username,
             users[1]->username);
      print_user_info(users[0]);
      break;

    default:
      return 0;
    
    };
  
  } while (true);

  return 0;
}

User *create_user(char *username) {
  static int id = 1;
  User *new_user = malloc(sizeof(User));
  new_user->id = id++;
  new_user->username = malloc(strlen(username) + 1);
  strcpy(new_user->username, username);

  new_user->following = NULL;
  new_user->followers = NULL;
  new_user->following_count = 0;
  new_user->followers_count = 0;

  return new_user;
}

void seed_users(User **users, bool is_with_followers) {
  char *name = malloc(128);

  for (int i = 0; i < USERS_NUM; i++) {
    sprintf(name, "user-%d", i + 1);
    users[i] = create_user(name);
    if (is_with_followers) {
      for (int j = 0; j < i; j++) {
        int rand_user = rand() % i + 1;
        follow(users[i], users[rand_user]);
      }
    }
  }
  free(name);
}

bool is_following(User *a, User *b) {
  UserNode *current = a->following;

  while (current) {
    if (current->user->id == b->id)
      return true;
    current = current->next;
  }
  return false;
}

bool remove_from_list(UserNode **list, User *user) {
  if (!*list)
    return false;

  UserNode *current = *list;
  UserNode *prev = NULL;

  while (current) {
    if (current->user->id == user->id) {
      if (prev)
        prev->next = current->next;
      else
        *list = current->next;

      free(current);
      return true;
    }

    prev = current;
    current = current->next;
  }
  return false;
}

void follow(User *a, User *b) {
  if (a == b || is_following(a, b))
    return;

  UserNode *following_node = malloc(sizeof(UserNode));
  following_node->user = b;
  following_node->next = a->following;

  a->following = following_node;
  a->following_count++;

  UserNode *follower_node = malloc(sizeof(UserNode));
  follower_node->user = a;
  follower_node->next = b->followers;

  b->followers = follower_node;
  b->followers_count++;
}

void unfollow(User *a, User *b) {
  if (a == b)
    return;

  if (!remove_from_list(&a->following, b))
    return;

  a->following_count--;
  remove_from_list(&b->followers, a);
  b->followers_count--;
}

User *find_user(User **users, char *username) {
  for (int i = 0; i < USERS_NUM; i++) {
    if (strcmp(users[i]->username, username) == 0) {
      print_user_info(users[i]);
      return users[i];
    }
  }
  printf("%s not found\n", username);
  return NULL;
}

void print_user_info(User *user) {
  printf("\n\n----- %s -----\n\n", user->username);
  printf("following %d users: ", user->following_count);
  UserNode *current = user->following;
  while (current) {
    printf("%s ", current->user->username);
    current = current->next;
  }
  printf("\n\n");

  printf("has %d followers: ", user->followers_count);
  current = user->followers;

  while (current) {
    printf("%s ", current->user->username);
    current = current->next;
  }
  printf("\n");
}

void print_users(User **users) {
  for (int i = 0; i < USERS_NUM; i++) {
    print_user_info(users[i]);
  }
}