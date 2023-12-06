#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

#define MAXLONG 200
#define MAXSHORT 4

int get_roots_range(double tmax, double dmax) {
  double sq = sqrt(tmax*tmax/4 - dmax);
  double root1 = tmax/2 - sq;
  double root2 = tmax/2 + sq;
  return (int)(floor(root2) - ceil(root1) + 1);
}

int main() {
  FILE* f;
  f = fopen("input/day06.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char line[MAXLONG] = {0};
  double times_p1[MAXSHORT] = {0};
  double distances_p1[MAXSHORT] = {0};
  double time_p2 = 0;
  double distance_p2 = 0;
  int n_races;

  for (int n = 0; n < 2; n++) {
    n_races = 0;
    fgets(line, MAXLONG, f);
    for (int i = 0; i < MAXLONG; i++) {
      if (line[i] == '\n' || line[i] == '\0') { break; }
      if (line[i] == ' ' && ((n == 0 && times_p1[n_races] != 0) || (n == 1 && distances_p1[n_races] != 0))) { n_races++; }
      if (is_digit(line[i])) {
        if ((n == 0 && times_p1[n_races] == 0) || (n == 1 && distances_p1[n_races] == 0)) {
          double val = (double)atoi(&line[i]);
          if (n == 0) {
            times_p1[n_races] = val;
          } else {
            distances_p1[n_races] = val;
          }
        }
        if (n == 0) {
          time_p2 = time_p2 * 10 + line[i] - '0';
        } else {
          distance_p2 = distance_p2 * 10 + line[i] - '0';
        }
      }
    }
  }
  n_races++;

  int p1 = 1;
  for (int i = 0; i < n_races; i++) { p1 *= get_roots_range(times_p1[i], distances_p1[i]); }

  printf("day 6 part 1: %d\n", p1);
  printf("day 6 part 2: %d\n", get_roots_range(time_p2, distance_p2));
  return 0;
}
