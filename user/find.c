//
// Created by dong on 2020/3/6.
//
#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char *
slash_pos(char *path) {
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--);
  p++;

  return p;
}

void find(char *path, char *name) {
  char buf[512], filename[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  strcpy(filename, slash_pos(path));

  switch(st.type){
    case T_FILE:
      if (strcmp(filename, name) == 0) {
        printf("%s\n", path);
      }
      break;

    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if ((de.inum == 0) || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        find(buf, name);
      }
      break;
  }
  close(fd);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: find path filename\n");
    exit();
  }

  if (argc < 3) {
    find(".", argv[1]);
    exit();
  }

  find(argv[1], argv[2]);
  exit();
}