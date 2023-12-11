#include <stdio.h>
#include <stdbool.h>
#include "utils.h"

#define MAXLEN 200

typedef enum Tile {
  None,
  Vertical,
  Horizontal,
  NorthEast,
  NorthWest,
  SouthEast,
  SouthWest,
} Tile;

typedef struct Node {
  Tile type;
  bool isloop;
  int distance;
  int neighbours[2];
} Node;

void get_start_type(Node *grid, int start, int n_cols, int n_total) {
  bool dirs[4] = {0};
  int n = 0;
  if (start / n_cols > 0 && (grid[start-n_cols].type == Vertical ||
                             grid[start-n_cols].type == SouthEast ||
                             grid[start-n_cols].type == SouthWest)) {
    dirs[0] = true;
    grid[start].neighbours[n++] = start-n_cols;
  }
  if (start / n_cols < n_total / n_cols - 1 && (grid[start+n_cols].type == Vertical ||
                                                grid[start+n_cols].type == NorthEast ||
                                                grid[start+n_cols].type == NorthWest)) {
    dirs[2] = true;
    grid[start].neighbours[n++] = start+n_cols;
  }
  if (start % n_cols > 0 && (grid[start-1].type == Horizontal ||
                             grid[start-1].type == SouthEast ||
                             grid[start-1].type == NorthEast)) {
    dirs[3] = true;
    grid[start].neighbours[n++] = start-1;
  }
  if (start % n_cols < n_cols - 1 && (grid[start+1].type == Horizontal ||
                                      grid[start+1].type == SouthWest ||
                                      grid[start+1].type == NorthWest)) {
    dirs[1] = true;
    grid[start].neighbours[n++] = start+1;
  }
  if (dirs[0]) {
    if (dirs[1]) {
      grid[start].type = NorthEast;
    } else if (dirs[2]) {
      grid[start].type = Vertical;
    } else if (dirs[3]) {
      grid[start].type = NorthWest;
    }
  } else if (dirs[1]) {
    if (dirs[2]) {
      grid[start].type = SouthEast;
    } else if (dirs[3]) {
      grid[start].type = Horizontal;
    }
  } else if (dirs[2] && dirs[3]) {
    grid[start].type = SouthWest;
  }
}

int get_loop(Node *grid, int start, int n_cols, int n_total) {
  bool visited[MAXLEN*MAXLEN] = {0};
  int queue[MAXLEN*MAXLEN] = {0};
  int queue_start = 0;
  int queue_end = 0;

  queue[queue_end++] = start;
  visited[start] = true;

  while (queue_start != queue_end) {
    int s = queue[queue_start++];
    grid[s].isloop = true;
    for (int i = 0; i < 2; i++) {
      int n = grid[s].neighbours[i];
      if (!visited[n]) {
        visited[n];
        queue[queue_end++] = n;
        visited[n] = true;
        grid[n].distance = grid[s].distance + 1;
      }
    }
  }
  return grid[queue[queue_end - 1]].distance;
}

int get_insides(Node *grid, int n_cols, int n_total) {
  int boundary_count = 0;
  int total = 0;
  for (int i = 0; i < n_total; i++) {
    if (i % n_cols == 0) { boundary_count = 0; }
    if (grid[i].isloop) {
      if (grid[i].type == Vertical || grid[i].type == SouthEast || grid[i].type == SouthWest) {
        boundary_count++;
      }
    } else if (boundary_count % 2 == 1) {
      total++;
    }
  }
  return total;
}

int main() {
  FILE* f;
  f = fopen("input/day10.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char line[MAXLEN] = {0};
  Node grid[MAXLEN*MAXLEN] = {0};
  int n_cols = 0;
  int n_total = 0;
  int start = -1;
  while (fgets(line, MAXLEN, f)) {
    if (n_cols == 0) { for (n_cols = 0; n_cols < MAXLEN; n_cols++) { if (line[n_cols] == '\n' || line[n_cols] == '\0') { break; } } }
    for (int c = 0; c < n_cols; c++) {
      switch (line[c]) {
        case '-':
          grid[n_total].type = Horizontal;
          grid[n_total].neighbours[0] = n_total-1;
          grid[n_total].neighbours[1] = n_total+1;
          n_total++;
          break;
        case '|':
          grid[n_total].type = Vertical;
          grid[n_total].neighbours[0] = n_total-n_cols;
          grid[n_total].neighbours[1] = n_total+n_cols;
          n_total++;
          break;
        case 'L':
          grid[n_total].type = NorthEast;
          grid[n_total].neighbours[0] = n_total-n_cols;
          grid[n_total].neighbours[1] = n_total+1;
          n_total++;
          break;
        case 'J':
          grid[n_total].type = NorthWest;
          grid[n_total].neighbours[0] = n_total-n_cols;
          grid[n_total].neighbours[1] = n_total-1;
          n_total++;
          break;
        case 'F':
          grid[n_total].type = SouthEast;
          grid[n_total].neighbours[0] = n_total+n_cols;
          grid[n_total].neighbours[1] = n_total+1;
          n_total++;
          break;
        case '7':
          grid[n_total].type = SouthWest;
          grid[n_total].neighbours[0] = n_total+n_cols;
          grid[n_total].neighbours[1] = n_total-1;
          n_total++;
          break;
        case '.':
          grid[n_total++].type = None;
          break;
        case 'S':
          start = n_total;
          grid[n_total++].type = None;
          break;
      }
    }
  }
  get_start_type(grid, start, n_cols, n_total);
  int p1 = get_loop(grid, start, n_cols, n_total);
  int p2 = get_insides(grid, n_cols, n_total);
  printf("day 10 part 1: %d\n", p1);
  printf("day 10 part 2: %d\n", p2);
  return 0;
}
