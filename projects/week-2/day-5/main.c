#include <stdio.h>
#include <stdlib.h>

typedef struct Color {
  unsigned int primary;
  unsigned int secondary;
} Color;

void writeFile();
void readFile();

int main() {
  writeFile();
  readFile();
  return 0;
}

void readFile() {
  FILE *pFileHandler = fopen("color.cpt", "rb");
  if (pFileHandler == NULL) {
    printf("error opening the file\n");
    return;
  }

  Color *pReadColor = (Color *)malloc(sizeof(Color));
  fread((void *)pReadColor, sizeof(Color), 1, pFileHandler);
  fclose(pFileHandler);

  printf("primary color: 0x%.8X\nsecondary color: 0x%.8X\n",
         pReadColor->primary, pReadColor->secondary);

  free(pReadColor);
}

void writeFile() {
  FILE *pFileHandler = fopen("color.cpt", "wb");
  if (pFileHandler == NULL) {
    printf("error opening the file\n");
    return;
  }

  Color *pColor = (Color *)malloc(sizeof(Color));

  pColor->primary = 0xFFFFFFFF;
  pColor->secondary = 0x000000FF;

  fwrite((void *)pColor, sizeof(Color), 1, pFileHandler);
  fclose(pFileHandler);
  free(pColor);
}