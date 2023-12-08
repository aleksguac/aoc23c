#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

#define MAXLONG 800
#define MAXSHORT 10

typedef struct Node {
  struct Node* left;
  struct Node* right;
} Node;

int strindex(char str[4], char list[][4], int n_list) {
  for (int i = 0; i < n_list; i++) {
    if (!strncmp(list[i], str, 4)) { return i; }
  }
  return -1;
}

long long gcd(long long a, long long b) {
  while (b != 0) {
    long long tmp = b;
    b = a % b;
    a = tmp;
  }
  return a;
}

long long lcm(long long a, long long b) {
  return a * b / gcd(a, b);
}

bool nodeptrlistContains(Node** list, Node* node, int n_list) {
  for (int i = 0; i < n_list; i++) {
    if (node == list[i]) { return true; }
  }
  return false;
}

bool longlistContains(long* list, long num, int n_list) {
  for (int i = 0; i < n_list; i++) {
    if (num == list[i]) { return true; }
  }
  return false;
}

void get_loops(Node** current, long* loops, int n_current, char* instructions, int n_instructions, Node** znodes, int n_znodes) {
  long n = 0;
  while (true) {
    for (int i = 0; i < n_current; i++) {
      if (n % n_instructions == 0 && loops[i] == 0 && nodeptrlistContains(znodes, current[i], n_znodes)) {
        loops[i] = n;
        if (!(longlistContains(loops, 0, n_current))) { return; }
      }
      if (instructions[n % n_instructions] == 'R') {
        current[i] = current[i]->right;
      } else {
        current[i] = current[i]->left;
      }
    }
    n++;
  }
}

int main() {
  FILE* f;
  f = fopen("input/day08.txt", "r");
  if (f == NULL) {
    printf("can't open file :(\n");
    return 1;
  }

  char instructions[MAXLONG] = {0};
  fgets(instructions, MAXLONG, f);
  int n_instructions = 0;
  for (int i = 0; i < MAXLONG; i++) {
    if (instructions[i] == '\n' || instructions[i] == '\0') { break; }
    n_instructions++;
  }

  char line[MAXLONG] = {0};
  fgets(line, MAXLONG, f); // empty line
  Node nodes[MAXLONG] = {0};

  char node_names[MAXLONG][4] = {0};
  int n_nodes = 0;

  while (fgets(line, MAXLONG, f)) {
    char name[4];
    char left[4];
    char right[4];
    sscanf(line, "%3s = (%3s, %3s)", name, left, right);

    int index = strindex(name, node_names, n_nodes);
    if (index == -1) {
      strncpy(node_names[n_nodes], name, 4);
      index = n_nodes;
      n_nodes++;
    }
    int lindex = strindex(left, node_names, n_nodes);
    if (lindex == -1) {
      strncpy(node_names[n_nodes], left, 4);
      lindex = n_nodes;
      n_nodes++;
    }
    int rindex = strindex(right, node_names, n_nodes);
    if (rindex == -1) {
      strncpy(node_names[n_nodes], right, 4);
      rindex = n_nodes;
      n_nodes++;
    }
    nodes[index].left = &nodes[lindex];
    nodes[index].right = &nodes[rindex];
  }

  Node* current = &nodes[strindex("AAA", node_names, n_nodes)];
  Node* zzz = &nodes[strindex("ZZZ", node_names, n_nodes)];
  int p1 = 0;
  while (current != zzz) {
    if (instructions[p1 % n_instructions] == 'R') {
      current = current->right;
    } else {
      current = current->left;
    }
    p1++;
  }

  Node* p2initial[MAXSHORT] = {0};
  int n_initial = 0;
  Node* znodes[MAXSHORT] = {0};
  int n_znodes = 0;
  for (int i = 0; i < n_nodes; i++) {
    if (node_names[i][2] == 'A') { p2initial[n_initial++] = &nodes[i]; }
    if (node_names[i][2] == 'Z') { znodes[n_znodes++] = &nodes[i]; }
  }
  long loops[MAXSHORT] = {0};
  get_loops(p2initial, loops, n_initial, instructions, n_instructions, znodes, n_znodes);

  long long p2 = loops[0];
  for (int i = 1; i < n_initial; i++) { p2 = lcm(p2, (long long)loops[i]); }

  printf("day 8 part 1: %d\n", p1);
  printf("day 8 part 2: %lld\n", p2);
  return 0;
}
