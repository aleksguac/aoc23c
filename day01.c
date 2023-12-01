#include <stdio.h>
#include <stdbool.h>
#include "utils.h"

#define MAXLEN 100

typedef struct DigitWord {
  char word[6];
  int len;
} DigitWord;

int word_to_digit(char* chars, int i) {
  DigitWord digits[10] = {{"zero", 4}, {"one", 3}, {"two", 3}, {"three", 5}, {"four", 4},
                          {"five", 4}, {"six", 3}, {"seven", 5}, {"eight", 5}, {"nine", 4}};
  for (int d = 0; d < 10; d++) {
    if (digits[d].len == 3 && i >= 2 && digits[d].word[0] == chars[i-2] &&
                                        digits[d].word[1] == chars[i-1] &&
                                        digits[d].word[2] == chars[i]) {
      return d;
    } else if (digits[d].len == 4 && i >= 3 && digits[d].word[0] == chars[i-3] &&
                                               digits[d].word[1] == chars[i-2] &&
                                               digits[d].word[2] == chars[i-1] &&
                                               digits[d].word[3] == chars[i]) {
      return d;
    } else if (digits[d].len == 5 && i >= 4 && digits[d].word[0] == chars[i-4] &&
                                               digits[d].word[1] == chars[i-3] &&
                                               digits[d].word[2] == chars[i-2] &&
                                               digits[d].word[3] == chars[i-1] &&
                                               digits[d].word[4] == chars[i]) {
      return d;
    }
  }
  return -1;
}

int main() {
  FILE* f;
  f = fopen("input/day01.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  int p1 = 0;
  int p2 = 0;
  while (true) {
    char line[MAXLEN] = {0};
    if (!fgets(line, MAXLEN, f)) { break; }
    
    int p1digits[] = {0, 0};
    int p2digits[] = {0, 0};
    for (int i = 0; i < MAXLEN; i++) {
      if (line[i] == '\0' || line[i] == '\n') { break; }
      int d = word_to_digit(line, i);
      if (d != -1) {
        if (p2digits[0] == 0) { p2digits[0] = d; }
        p2digits[1] = d;
      }
      if (is_digit(line[i])) {
        if (p1digits[0] == 0) { p1digits[0] = line[i] - '0'; }
        if (p2digits[0] == 0) { p2digits[0] = line[i] - '0'; }
        p1digits[1] = line[i] - '0';
        p2digits[1] = line[i] - '0';
      }
    }
    p1 += p1digits[0]*10 + p1digits[1];
    p2 += p2digits[0]*10 + p2digits[1];
  }
  printf("day 1 part 1: %d\n", p1);
  printf("day 1 part 2: %d\n", p2);
  return 0;
}
