#include <stdio.h>

#define symbol_dot 0
#define symbol_hash 1
#define symbol_at 2
#define symbol_and 3

#define color_black 0
#define color_red 1
#define color_green 2
#define color_yellow 3
#define color_blue 4
#define color_white 5
#define color_purple 6
#define color_gold 7

#define ANSI_BLACK "\x1b[30m"
#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_PURPLE "\x1b[35m"
#define ANSI_WHITE "\x1b[37m"
#define ANSI_GOLD "\x1b[93m"

#define ANSI_BG_BLACK "\x1b[40m"
#define ANSI_BG_RED "\x1b[41m"
#define ANSI_BG_GREEN "\x1b[42m"
#define ANSI_BG_YELLOW "\x1b[43m"
#define ANSI_BG_BLUE "\x1b[44m"
#define ANSI_BG_PURPLE "\x1b[45m"
#define ANSI_BG_WHITE "\x1b[47m"
#define ANSI_BG_GOLD "\x1b[103m"

typedef struct Pixel {
  unsigned char visible : 1;
  unsigned char color : 3;
  unsigned char background : 1;
  unsigned char symbol : 3;
} Pixel;

#define A 0x05
#define B 0x6F
#define C 0x49

unsigned char board[24][24] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A, A, A, A, A, A, A, A, A, A, A, A, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A, 0},
    {0, 0, 0, 0, C, C, C, C, C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A, 0},
    {0, 0, 0, 0, C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A, 0},
    {0, 0, 0, 0, C, C, C, C, C, 0, 0, 0, 0, 0, 0, 0, B, B, B, B, 0, 0, A, 0},
    {0, 0, 0, 0, C, 0, 0, 0, C, 0, 0, 0, 0, 0, 0, 0, B, 0, 0, B, 0, 0, A, 0},
    {0, 0, 0, 0, C, 0, 0, 0, C, A, A, A, A, A, A, A, A, 0, 0, B, B, B, B, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

char get_symbol(unsigned char symbol) {
  switch (symbol) {
  case symbol_hash:
    return '#';
  case symbol_dot:
    return '.';
  case symbol_at:
    return '@';
  case symbol_and:
    return '&';
  default:
    return '.';
  }
}

char *get_ansi_color(unsigned char color) {
  switch (color) {
  case color_black:
    return ANSI_BLACK;
  case color_red:
    return ANSI_RED;
  case color_green:
    return ANSI_GREEN;
  case color_yellow:
    return ANSI_YELLOW;
  case color_blue:
    return ANSI_BLUE;
  case color_white:
    return ANSI_WHITE;
  case color_purple:
    return ANSI_PURPLE;
  case color_gold:
    return ANSI_GOLD;
  default:
    return ANSI_WHITE;
  }
}

char *get_ansi_bg(unsigned char color) {
  switch (color) {
  case color_black:
    return ANSI_BG_BLACK;
  case color_red:
    return ANSI_BG_RED;
  case color_green:
    return ANSI_BG_GREEN;
  case color_yellow:
    return ANSI_BG_YELLOW;
  case color_blue:
    return ANSI_BG_BLUE;
  case color_white:
    return ANSI_BG_WHITE;
  case color_purple:
    return ANSI_BG_PURPLE;
  case color_gold:
    return ANSI_BG_GOLD;
  default:
    return ANSI_BG_BLACK;
  }
}

int main() {

  for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 24; j++) {

      unsigned char value = board[i][j];
      Pixel *pixel = (Pixel *)&value;

      if (!pixel->visible) {
        printf(" ");
        continue;
      }

      if (pixel->background) {
        printf("%s%c", get_ansi_bg(pixel->color), get_symbol(pixel->symbol));
      } else {
        printf("%s%c", get_ansi_color(pixel->color), get_symbol(pixel->symbol));
      }
    }
    printf("\n");
  }
  return 0;
}