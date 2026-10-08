#ifndef LS_H
#define LS_H
#include <sys/stat.h>

// Struct lưu các cờ options
typedef struct {
  int a, l, i, R, t, r, S;
} ls_opts;

// Struct lưu thông tin 1 file để phục vụ sắp xếp (sorting)
typedef struct {
  char name[256];
  char full_path[1024];
  struct stat file_stat;
} FileInfo;

void list_directory(const char *path, ls_opts opts);

#endif