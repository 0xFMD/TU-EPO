#include <stdio.h>
const int ARRAY_SIZE = 5;

void writeStoryToFile() {
  FILE *fh = fopen("story.txt", "w");
  fprintf(fh, "Hello11 Hello12 Hello13\nHello21 Hello22 Hello23\nHello31"
              " Hello32 Hello33");
  fclose(fh);
}

void countWords(FILE *fh) {
  if (fh)
    return;
  int count = 0;

  char c;
  while ((c = fgetc(fh)) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t')
      count++;
  }

  printf("Number of words: %d\n", count);
}

void writeFpFile() {
  FILE *fh = fopen("floats.bin", "wb");
  if (!fh)
    return;

  float ar[] = {1.1, 2.1, 3.1, 4.1, 5.1};
  fwrite(ar, sizeof(float), ARRAY_SIZE, fh);
  fclose(fh);
}

void sumFpFile(FILE *fh) {
  if (!fh)
    return;
  float ar[ARRAY_SIZE];
  float sum = 0;
  fread(ar, sizeof(float), ARRAY_SIZE, fh);
  for (int i = 0; i < ARRAY_SIZE; i++)
    sum += ar[i];

  printf("Sum of floats: %.2f\n", sum);
}

int main() {
  writeStoryToFile();
  FILE *f1 = fopen("story.txt", "r");
  if (!f1)
    return 1;
  countWords(f1);
  fclose(f1);

  writeFpFile();
  FILE *f2 = fopen("floats.bin", "rb");
  if (!f2)
    return 1;
  sumFpFile(f2);
  fclose(f2);
}