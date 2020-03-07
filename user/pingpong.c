//
// Created by dong on 2020/3/5.
//
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int parent_fd[2];
  int child_fd[2];
  char buf[] = "ping pong";

  if (-1 == pipe(parent_fd)) {
    printf("ops, error during the pipe making.\n");
    exit();
  }
  if (-1 == pipe(child_fd)) {
    printf("ops, error during the pipe making.\n");
    exit();
  }
  int child_pid = fork();
  if (child_pid == -1) {
    printf("ops, error during the fork.\n");
    exit();
  }

  if (child_pid > 0) {
    int pid = getpid();
    close(parent_fd[0]);
    close(child_fd[1]);

    write(parent_fd[1], buf, 1);
    read(child_fd[0], buf, 1);
    printf("%d: received pong\n", pid);

    close(parent_fd[1]);
    close(child_fd[0]);
  } else {
    int pid = getpid();
    close(parent_fd[1]);
    close(child_fd[0]);

    read(parent_fd[0], buf, 1);
    printf("%d: received ping\n", pid);
    write(child_fd[1], buf, 1);

    close(parent_fd[0]);
    close(child_fd[1]);
  }

  exit();
}
