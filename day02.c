#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

#define MAXLEN 200

int main() {
  FILE* f;
  f = fopen("input/day02.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char line[MAXLEN] = {0};
  int p1 = 0;
  int p2 = 0;
  int val = 0;
  bool p1possible;
  int game_id;

  while (fgets(line, MAXLEN, f)) {
    game_id = atoi(&line[5]);
    p1possible = true;
    int p2minimums[] = {0, 0, 0};
    for (int i = 0; i < MAXLEN; i++) {
      if (line[i] == '\0') { break; }
      if (line[i] == ':' || line[i] == ';' || line[i] == ',') { val = 0; }
      if (val == 0 && is_digit(line[i])) { val = atoi(&line[i]); }
      if (val > 0) {
        if (line[i] == 'r') {
          if (val > 12) { p1possible = false; }
          if (val > p2minimums[0]) { p2minimums[0] = val; }
          i += 2;
        } else if (line[i] == 'g') {
          if (val > 13) { p1possible = false; }
          if (val > p2minimums[1]) { p2minimums[1] = val; }
          i += 4;
        } else if (line[i] == 'b') {
          if (val > 14) { p1possible = false; }
          if (val > p2minimums[2]) { p2minimums[2] = val; }
          i += 3;
        }
      }
    }
    if (p1possible) { p1 += game_id; }
    p2 += p2minimums[0] * p2minimums[1] * p2minimums[2];
  }
  printf("day 2 part 1: %d\n", p1);
  printf("day 2 part 2: %d\n", p2);
  return 0;
}
