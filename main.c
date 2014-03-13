// main.c
// top-level code for the particle simulation

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

int main(int argv, char **argc) {
  runsim(parseconfig(argc[1]), parseinit(argc[2]));
  exit(0);
}
