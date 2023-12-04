#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

#define MAXLEN 300

bool num_in_list(int *list, int n_list, int num) {
  for (int i = 0; i < n_list; i++) {
    if (num == list[i]) { return true; }
  }
  return false;
}

void add_to_total(int *winners_per_card, int i, int *n_cards) {
  (*n_cards)++;
  for (int j = i + 1; j <= i + winners_per_card[i]; j++) {
    add_to_total(winners_per_card, j, n_cards);
  }
}

int main() {
  FILE* f;
  f = fopen("input/day04.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char line[MAXLEN] = {0};
  int p1 = 0;
  int p2counts[MAXLEN] = {0};
  int n_p2counts = 0;
  while (fgets(line, MAXLEN, f)) {
    int winners[MAXLEN] = {0};
    int n_winners = 0;
    int mynums[MAXLEN] = {0};
    int n_mynums = 0;
    bool after_id = false;
    bool after_winners = false;
    for (int i = 0; i < MAXLEN; i++) {
      if (line[i] == '\0' || line[i] == '\n') { break; }
      if (line[i] == ':') { after_id = true; }
      if (after_id) {
        if (is_digit(line[i])) {
          if (after_winners) {
            mynums[n_mynums++] = atoi(&line[i]);
          } else {
            winners[n_winners++] = atoi(&line[i]);
          }
          i++;
        } else if (line[i] == '|') {
          after_winners = true;
        }
      }
    }
    int total = 0;
    for (int i = 0; i < n_mynums; i++) {
      if (num_in_list(winners, n_winners, mynums[i])) {
        total++;
      }
    }
    if (total > 0) { p1 += 1 << (total - 1); }
    p2counts[n_p2counts++] = total;
  }

  int p2 = 0;
  for (int i = 0; i < n_p2counts; i++) {
    add_to_total(p2counts, i, &p2);
  }

  printf("day 4 part 1: %d\n", p1);
  printf("day 4 part 2: %d\n", p2);
  return 0;
}
