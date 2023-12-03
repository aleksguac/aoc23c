#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

#define MAXLEN 150
#define MAXVALS 1300

typedef struct Values {
  int numbers[MAXVALS];
  int lens[MAXVALS];
  int positions[MAXVALS];
  unsigned n_values;
} Values;

typedef struct Symbols {
  int types[MAXVALS];
  int positions[MAXVALS];
  unsigned n_symbols;
  int vals[MAXVALS][8];
  unsigned n_vals[MAXVALS];
} Symbols;

bool symbols_contains(Symbols *symbols, int position, int value) {
  for (int s = 0; s < symbols->n_symbols; s++) {
    if (symbols->positions[s] == position) {
      symbols->vals[s][symbols->n_vals[s]++] = value; // add to symbol's adjacent values
      return true;
    }
  }
  return false;
}

int main() {
  FILE* f;
  f = fopen("input/day03.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char line[MAXLEN] = {0};
  Values values = {0};
  Symbols symbols = {0};
  int line_num = 0;
  while (fgets(line, MAXLEN, f)) {
    for (int c = 0; c < MAXLEN; c++) {
      if (line[c] == '\0' || line[c] == '\n') { break; }
      if (is_digit(line[c])) {
        int val = atoi(&line[c]);
        int len = 0;
        int val_tmp = val;
        while (val_tmp > 0) { len++; val_tmp /= 10; }
        values.numbers[values.n_values] = val;
        values.lens[values.n_values] = len;
        values.positions[values.n_values] = line_num * MAXLEN + c;
        values.n_values++;
        c += len - 1;
      } else if (line[c] != '.') {
        symbols.types[symbols.n_symbols] = (line[c] == '*') ? 2 : 1;
        symbols.positions[symbols.n_symbols] = line_num * MAXLEN + c;
        symbols.n_symbols++;
      }
    }
    line_num++;
  }

  // iterate through all values and check for neighbouring symbols, add to p1 total if present
  int p1 = 0;
  for (int v = 0; v < values.n_values; v++) {
    bool symbol = false;
    for (int i = values.positions[v] - 1; i <= values.positions[v] + values.lens[v]; i++) {
      // if (values.numbers[v] == 539) { printf("%d %d\n", values.positions[v], i); }
      if (symbols_contains(&symbols, i, values.numbers[v]) ||
          symbols_contains(&symbols, i-MAXLEN, values.numbers[v]) ||
          symbols_contains(&symbols, i+MAXLEN, values.numbers[v])) { symbol = true; }
    }
    if (symbol) { p1 += values.numbers[v]; }
    if (values.lens[v] > 3) { printf("%d\n", values.lens[v]); }
  }
  // iterate through symbols and add all gear ratios to p2 total
  int p2 = 0;
  for (int s = 0; s < symbols.n_symbols; s++) {
    if (symbols.n_vals[s] == 2 && symbols.types[s] == 2) { p2 += symbols.vals[s][0] * symbols.vals[s][1]; }
  }
  printf("day 3 part 1: %d\n", p1);
  printf("day 3 part 2: %d\n", p2);
  return 0;
}
