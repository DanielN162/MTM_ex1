#include <stdio.h>

void printArray(char const **strings, int size) {
  for (int i = 0; i < size; ++i) {
    printf("%s\n", strings[i]);
  }
}

int main(int argc, char const *argv[]) {
  char const *strings[4] = {"hello to you", "and", "everyone", "else"};
  printArray(strings, 4);
  /* printArray(argv + 1, argc - 1); */
  return 0;
}

