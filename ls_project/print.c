#include "print.h"
#include "ls.h"
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <time.h>

void print_file_info(FileInfo *file, ls_opts opts) {
  if (opts.i) {
    printf("%7lu ", (unsigned long)file->file_stat.st_ino);
  }

  if (opts.l) {
    // Phân quyền
    printf((S_ISDIR(file->file_stat.st_mode)) ? "d" : "-");
    printf((file->file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file->file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file->file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file->file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file->file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file->file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file->file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file->file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file->file_stat.st_mode & S_IXOTH) ? "x" : "-");

    printf(" %2lu ", (unsigned long)file->file_stat.st_nlink);

    struct passwd *pw = getpwuid(file->file_stat.st_uid);
    struct group *gr = getgrgid(file->file_stat.st_gid);
    printf(" %s %s ", pw ? pw->pw_name : "UID", gr ? gr->gr_name : "GID");

    printf("%8lld ", (long long)file->file_stat.st_size);

    char time_buf[64];
    struct tm *tm_info = localtime(&file->file_stat.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
    printf("%s ", time_buf);
  }

  printf("%s", file->name);
  if (opts.l)
    printf("\n");
  else
    printf("  ");
}