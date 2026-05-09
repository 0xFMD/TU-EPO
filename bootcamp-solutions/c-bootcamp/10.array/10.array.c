#include <stdio.h>

int main() {

  float scores[] = {78.0, 85.5, 62.0, 90.0, 74.5};
  const int scores_len = sizeof(scores) / sizeof(float);
  float total = 0;
  int lowest_index = 0;

  printf("Original scores:\n");

  for (int i = 0; i < scores_len; i++)
    printf("%.1f ", scores[i]);

  for (int i = 0; i < scores_len; i++) {
    if (scores[i] < scores[i + 1])
      lowest_index = i;
  }

  scores[lowest_index] = 80.5;

  printf("\nUpdated scores:\n");

  for (int i = 0; i < scores_len; i++) {
    total += scores[i];
    printf("%.1f ", scores[i]);
  }

  const float avg = total / scores_len;

  printf("\naverage score: %.2f\n", avg);

  printf("number of students: %d\n", scores_len);
}