#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

#define MAXLONG 300
#define MAXMID 100
#define MAXSHORT 10

typedef struct Conversion {
  long src_start;
  long dst_start;
  long range;
} Conversion;

typedef struct ConversionStep {
  Conversion conversions[MAXMID];
  int n;
} ConversionStep;

int cmp_conversion(const void *a, const void *b) {
  const Conversion* ac = a;
  const Conversion* bc = b;
  return (ac->src_start > bc->src_start) ? 1 :
         (ac->src_start < bc->src_start) ? -1 : 0;
}

int get_index_between(ConversionStep *step, long n) {
  for (int i = 0; i < step->n; i++) {
    if (step->conversions[i].src_start <= n && (i == step->n - 1 || step->conversions[i+1].src_start > n)) {
      return i;
    }
  }
  return -1;
}

long part1(ConversionStep *conversions, int n_all, long *seeds, int n_seeds) {
  long values[MAXMID] = {0};
  for (int i = 0; i < n_seeds; i++) { values[i] = seeds[i]; }
  for (int c = 0; c < n_all; c++) {
    for (int s = 0; s < n_seeds; s++) {
      int d = get_index_between(&conversions[c], values[s]);
      if (d != -1 && conversions[c].conversions[d].src_start + conversions[c].conversions[d].range > values[s]) {
        values[s] = conversions[c].conversions[d].dst_start + values[s] - conversions[c].conversions[d].src_start;
      }
    }
  }
  long res = values[0];
  for (int i = 0; i < n_seeds; i++) {
    if (values[i] < res) { res = values[i]; }
  }
  return res;
}

long part2(ConversionStep *conversions, int n_all, long *seeds, int n_seeds) {
  long values[MAXLONG] = {0};
  for (int i = 0; i < n_seeds; i++) { values[i] = seeds[i]; }
  for (int c = 0; c < n_all; c++) {
    long next[MAXLONG] = {0};
    int n_next = 0;
    for (int s = 0; s < n_seeds/2; s++) {
      long current_start = values[2*s]; // inclusive
      long current_range = values[2*s+1];
      long current_end = current_start + current_range; // exclusive
      while (true) {
        int d = get_index_between(&conversions[c], current_start);
        if (d == -1) {
          if (current_end <= conversions[c].conversions[0].src_start) {
            //      .......          ...     ............
            // ...
            next[n_next++] = current_start;
            next[n_next++] = current_range;
            break;
          } else {
            //      .......          ...     ............
            // ..........(or longer)
            next[n_next++] = current_start;
            next[n_next++] = conversions[c].conversions[0].src_start - current_start;
            current_start = conversions[c].conversions[0].src_start;
            current_range = current_end - current_start;
            d = 0;
          }
        }
        long conv_src_start = conversions[c].conversions[d].src_start; // inclusive
        long conv_src_range = conversions[c].conversions[d].range;
        long conv_src_end = conv_src_start + conv_src_range; // exclusive
        long conv_dst_end = conversions[c].conversions[d].dst_start; // inclusive
        if (current_start < conv_src_end) {
          if (current_end <= conv_src_end) {
            //      .......          ...     ............
            //        ....
            next[n_next++] = conv_dst_end + current_start - conv_src_start;
            next[n_next++] = current_range;
            break;
          } else {
            //      .......          ...     ............
            //        .........(or longer)
            next[n_next++] = conv_dst_end + current_start - conv_src_start;
            next[n_next++] = conv_src_end - current_start;
            current_start = conv_src_end;
            current_range = current_end - current_start;
          }
        } else {
          if (d == conversions[c].n - 1 || current_end <= conversions[c].conversions[d+1].src_start) {
            //      .......          ...     ............
            //               .....
            next[n_next++] = current_start;
            next[n_next++] = current_range;
            break;
          } else {
            //      .......          ...     ............
            //               ..........(or longer)
            next[n_next++] = current_start;
            next[n_next++] = conversions[c].conversions[d+1].src_start - current_start;
            current_start = conversions[c].conversions[d+1].src_start;
            current_range = current_end - current_start;
          }
        }
      }
    }
    for (int s = 0; s < MAXLONG; s++) {
      if (s < n_next) {
        values[s] = next[s];
      } else {
        values[s] = 0;
      }
    }
    n_seeds = n_next;
  }
  long res = values[0];
  for (int i = 0; i < n_seeds; i++) {
    if (i % 2 == 0 && values[i] < res) { res = values[i]; }
  }
  return res;
}

int main() {
  FILE* f;
  f = fopen("input/day05.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char line[MAXLONG] = {0};

  fgets(line, MAXLONG, f); // get seed list
  long seeds[MAXMID] = {0};
  int n_seeds = 0;
  for (int i = 0; i < MAXLONG; i++) {
    if (line[i] == '\n' || line[i] == '\0') { n_seeds++; break; }
    if (is_digit(line[i]) && seeds[n_seeds] == 0) { seeds[n_seeds] = atol(&line[i]); }
    if (line[i] == ' ' && seeds[n_seeds] != 0) { n_seeds++; }
  }
  fgets(line, MAXLONG, f); // should just be the empty line after the seed list

  ConversionStep all_conversions[MAXSHORT] = {0};
  int n_all = 0;
  while (fgets(line, MAXLONG, f)) {
    if (line[0] == '\n') {
      n_all++;
      continue; // go to next conversion after empty line
    }
    if (!is_digit(line[0])) { continue; } // skip titles
    int n_val = 0;
    long vals[3] = {0};
    for (int i = 0; i < MAXLONG; i++) {
      if (line[i] == '\n' || line[i] == '\0') { break; }
      if (line[i] == ' ') { n_val++; }
      if (is_digit(line[i]) && vals[n_val] == 0) {
        vals[n_val] = atol(&line[i]);
      }
    }
    all_conversions[n_all].conversions[all_conversions[n_all].n].dst_start = vals[0];
    all_conversions[n_all].conversions[all_conversions[n_all].n].src_start = vals[1];
    all_conversions[n_all].conversions[all_conversions[n_all].n].range = vals[2];
    all_conversions[n_all].n++;
  }
  n_all++;
  for (int i = 0; i < n_all; i++) { qsort(all_conversions[i].conversions, all_conversions[i].n, sizeof(Conversion), cmp_conversion); }

  printf("day 5 part 1: %ld\n", part1(all_conversions, n_all, seeds, n_seeds));
  printf("day 5 part 2: %ld\n", part2(all_conversions, n_all, seeds, n_seeds));
  return 0;
}
