//
// Created by dong on 2020/3/6.
//
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: xargs command\n");
    exit();
  }

  char *cmd = argv[1];
  char* args[MAXARG];
  int arg_id = 0;
  for (int i = 1; i < argc; ++i) {
    args[arg_id++] = argv[i];
  }
  int init_arg_id = arg_id;

  int n;
  char ch;
  int buf_id = 0;
  char argbuf[512];
  while ((n = read(0, &ch, sizeof(ch))) > 0) {
    if (ch == ' ' || ch == '\n') {
      argbuf[buf_id] = '\0';
      char *new_arg = (char*)malloc((buf_id + 1) * sizeof(char));
      strcpy(new_arg, argbuf);
      args[arg_id++] = new_arg;
      buf_id = 0;
      if (ch == '\n') {
        int child_pid = fork();
        if (child_pid > 0) {
          for (int i = init_arg_id; i < arg_id; ++i) {
            free(args[i]);
            args[i] = 0;
          }
          arg_id = init_arg_id;
          wait();
        } else {
          exec(cmd, args);
        }
      }
    } else {
      argbuf[buf_id++] = ch;
    }

  }

  exit();
}