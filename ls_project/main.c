#include "ls.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int opt;
  ls_opts opts = {0};

  // Bắt các cờ (a, l, i, R, t, r)
  while ((opt = getopt(argc, argv, "aliRtr")) != -1) {
    switch (opt) {
    case 'a':
      opts.a = 1;
      break;
    case 'l':
      opts.l = 1;
      break;
    case 'i':
      opts.i = 1;
      break;
    case 'R':
      opts.R = 1;
      break;
    case 't':
      opts.t = 1;
      break;
    case 'r':
      opts.r = 1;
      break;
    default:
      fprintf(stderr, "Usage: %s [-alirRt] [dir...]\n", argv[0]);
      exit(1);
    }
  }

  if (optind == argc) {
    list_directory(".", opts);
  } else {
    for (int i = optind; i < argc; i++) {
      if (argc - optind > 1)
        printf("%s:\n", argv[i]);
      list_directory(argv[i], opts);
      if (i < argc - 1)
        printf("\n");
    }
  }
  return 0;
}