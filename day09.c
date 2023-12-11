#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

#define MAXLEN 200

int main() {
  FILE* f;
  f = fopen("input/day09.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char line[MAXLEN] = {0};
  long p1 = 0;
  long p2 = 0;

  while (fgets(line, MAXLEN, f)) {
    long vals_line[MAXLEN][MAXLEN] = {0};
    int n_vals_line = 0;
    int same = false;
    for (int c = 0; c < MAXLEN; c++) {
      if (line[c] == '\n' || line[c] == '\0') { n_vals_line++; break; }
      if (line[c] == ' ') { same = false; n_vals_line++; }
      if ((line[c] == '-' || is_digit(line[c])) && !same) {
        vals_line[0][n_vals_line] = atoi(&line[c]);
        same = true;
      }
    }
    bool allzeros;
    int order = 0;
    for (int row = 1; row < MAXLEN; row++) {
      allzeros = true;
      for (int i = 0; i < n_vals_line - row; i++) {
        vals_line[row][i] = vals_line[row-1][i+1] - vals_line[row-1][i];
        if (vals_line[row][i] != 0) { allzeros = false; }
      }
      if (allzeros) { order = row; break; }
    }

    long p1part = 0;
    long p2part = 0;
    for (int i = order - 1; i >= 0; i--) {
      p1part = vals_line[i][n_vals_line-i-1] + p1part;
      p2part = vals_line[i][0] - p2part;
    }
    p1 += p1part;
    p2 += p2part;
  }
  printf("day 9 part 1: %ld\n", p1);
  printf("day 9 part 2: %ld\n", p2);
  return 0;
}
