#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "ls.h"
#include "print.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hàm so sánh để qsort (Sắp xếp theo thời gian hoặc Alphabet)
int cmp_files(const void *a, const void *b, void *arg) {
  FileInfo *f1 = (FileInfo *)a;
  FileInfo *f2 = (FileInfo *)b;
  ls_opts *opts = (ls_opts *)arg;
  int res = 0;

  if (opts->t) {
    // Sắp xếp theo thời gian mới nhất (mtime)
    if (f1->file_stat.st_mtime < f2->file_stat.st_mtime)
      res = 1;
    else if (f1->file_stat.st_mtime > f2->file_stat.st_mtime)
      res = -1;
    else
      res = strcmp(f1->name, f2->name);
  } else {
    // Mặc định sắp xếp theo Alphabet (lexicographical order)
    res = strcmp(f1->name, f2->name);
  }
  return opts->r ? -res : res; // Đảo ngược nếu có cờ -r
}

// Wrapper cho qsort_r (trên Linux)
int cmp_wrapper(const void *a, const void *b, void *arg) {
  return cmp_files(a, b, arg);
}

void list_directory(const char *path, ls_opts opts) {
  DIR *dp = opendir(path);
  if (!dp) {
    perror("myls: opendir");
    return;
  }

  struct dirent *entry;
  int capacity = 100;
  int count = 0;
  FileInfo *files = malloc(capacity * sizeof(FileInfo));

  // Đọc toàn bộ file vào mảng
  while ((entry = readdir(dp)) != NULL) {
    if (!opts.a && entry->d_name[0] == '.')
      continue;

    if (count >= capacity) {
      capacity *= 2;
      files = realloc(files, capacity * sizeof(FileInfo));
    }

    strcpy(files[count].name, entry->d_name);
    snprintf(files[count].full_path, sizeof(files[count].full_path), "%s/%s",
             path, entry->d_name);
    lstat(files[count].full_path, &files[count].file_stat);
    count++;
  }
  closedir(dp);

  // Sắp xếp mảng (qsort_r hỗ trợ truyền thêm param opts)
  qsort_r(files, count, sizeof(FileInfo), cmp_wrapper, &opts);

  // In thư mục hiện tại
  if (opts.R)
    printf("\n%s:\n", path);
  for (int i = 0; i < count; i++) {
    print_file_info(&files[i], opts);
  }
  if (!opts.l)
    printf("\n");

  // Xử lý đệ quy (-R)
  if (opts.R) {
    for (int i = 0; i < count; i++) {
      if (S_ISDIR(files[i].file_stat.st_mode) &&
          strcmp(files[i].name, ".") != 0 && strcmp(files[i].name, "..") != 0) {
        list_directory(files[i].full_path, opts);
      }
    }
  }
  free(files);
}