#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

#define MAXLEN 1000
#define MAXLINE 6

typedef struct Hand {
  int cards1[5];
  int cards2[5];
  int rank;
  int hand1; // 7 five 6 four 5 full 4 three 3 two pair 2 one pair 1 high card 0 unknown
  int hand2; // 7 five 6 four 5 full 4 three 3 two pair 2 one pair 1 high card 0 unknown
} Hand;

void gethand1(Hand* hand) {
  int counts[15] = {0};
  for (int h = 0; h < 5; h++) { counts[hand->cards1[h]]++; }
  int sets[4] = {0}; // twos, threes, fours, fives
  for (int i = 2; i < 15; i++) { if (counts[i] > 1) { sets[counts[i] - 2]++; } }

  if (sets[3] == 1) {
    hand->hand1 = 7;
  } else if (sets[2] == 1) {
    hand->hand1 = 6;
  } else if (sets[1] == 1 && sets[0] == 1) {
    hand->hand1 = 5;
  } else if (sets[1] == 1) {
    hand->hand1 = 4;
  } else if (sets[0] == 2) {
    hand->hand1 = 3;
  } else if (sets[0] == 1) {
    hand->hand1 = 2;
  } else {
    hand->hand1 = 1;
  }
}

void gethand2(Hand* hand) {
  int counts[15] = {0};
  for (int h = 0; h < 5; h++) { counts[hand->cards2[h]]++; }
  int sets[4] = {0}; // 0 twos, 1 threes, 2 fours, 3 fives
  for (int i = 2; i < 15; i++) { if (counts[i] > 1) { sets[counts[i] - 2]++; } }

  switch (counts[1]) {
    case 5:
    case 4:
      hand->hand2 = 7;
      break;
    case 3:
      if (sets[0] == 1) {
        hand->hand2 = 7;
      } else {
        hand->hand2 = 6;
      }
      break;
    case 2:
      if (sets[1] == 1) {
        hand->hand2 = 7;
      } else if (sets[0] == 1) {
        hand->hand2 = 6;
      } else {
        hand->hand2 = 4;
      }
      break;
    case 1:
      if (sets[2] == 1) {
        hand->hand2 = 7;
      } else if (sets[1] == 1) {
        hand->hand2 = 6;
      } else if (sets[0] == 2) {
        hand->hand2 = 5;
      } else if (sets[0] == 1) {
        hand->hand2 = 4;
      } else {
        hand->hand2 = 2;
      }
      break;
    case 0:
      if (sets[3] == 1) {
        hand->hand2 = 7;
      } else if (sets[2] == 1) {
        hand->hand2 = 6;
      } else if (sets[1] == 1 && sets[0] == 1) {
        hand->hand2 = 5;
      } else if (sets[1] == 1) {
        hand->hand2 = 4;
      } else if (sets[0] == 2) {
        hand->hand2 = 3;
      } else if (sets[0] == 1) {
        hand->hand2 = 2;
      } else {
        hand->hand2 = 1;
      }
      break;
  }
}

int cmp_hand1(const void* a, const void* b) {
  const Hand* ahand = (Hand*)a;
  const Hand* bhand = (Hand*)b;
  if (ahand->hand1 == bhand->hand1) {
    for (int i = 0; i < 5; i++) {
      if (ahand->cards1[i] != bhand->cards1[i] || i == 4) {
        return ahand->cards1[i] - bhand->cards1[i];
      }
    }
  }
  return ahand->hand1 - bhand->hand1;
}

int cmp_hand2(const void* a, const void* b) {
  const Hand* ahand = (Hand*)a;
  const Hand* bhand = (Hand*)b;
  if (ahand->hand2 == bhand->hand2) {
    for (int i = 0; i < 5; i++) {
      if (ahand->cards2[i] != bhand->cards2[i] || i == 4) {
        return ahand->cards2[i] - bhand->cards2[i];
      }
    }
  }
  return ahand->hand2 - bhand->hand2;
}

int main() {
  FILE* f;
  f = fopen("input/day07.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char line[MAXLEN] = {0};

  Hand hands[MAXLEN] = {0};
  int n_hands = 0;

  while (fgets(line, MAXLEN, f)) {
    for (int c = 0; c < MAXLEN; c++) {
      if (c < MAXLINE) {
        if (is_digit(line[c])) {
          hands[n_hands].cards1[c] = line[c] - '0';
          hands[n_hands].cards2[c] = line[c] - '0';
        } else {
          switch (line[c]) {
            case 'T':
              hands[n_hands].cards1[c] = 10;
              hands[n_hands].cards2[c] = 10;
              break;
            case 'J':
              hands[n_hands].cards1[c] = 11;
              hands[n_hands].cards2[c] = 1;
              break;
            case 'Q':
              hands[n_hands].cards1[c] = 12;
              hands[n_hands].cards2[c] = 12;
              break;
            case 'K':
              hands[n_hands].cards1[c] = 13;
              hands[n_hands].cards2[c] = 13;
              break;
            case 'A':
              hands[n_hands].cards1[c] = 14;
              hands[n_hands].cards2[c] = 14;
              break;
          }
        }
      } else if (c == MAXLINE) {
        hands[n_hands].rank = atoi(&line[c]);
        gethand1(&hands[n_hands]);
        gethand2(&hands[n_hands]);
        n_hands++;
        break;
      }
    }
  }

  qsort(hands, n_hands, sizeof(Hand), cmp_hand1);
  int p1 = 0;
  for (int i = 0; i < n_hands; i++) { p1 += (i+1)*hands[i].rank; }
  
  qsort(hands, n_hands, sizeof(Hand), cmp_hand2);
  int p2 = 0;
  for (int i = 0; i < n_hands; i++) { p2 += (i+1)*hands[i].rank; }

  printf("day 7 part 1: %d\n", p1);
  printf("day 7 part 2: %d\n", p2);
  return 0;
}
