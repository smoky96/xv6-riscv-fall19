//
// Created by dong on 2020/3/5.
//
#include "kernel/types.h"
#include "user/user.h"

int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

void fork_children(int child_id, int* parend_fd) {
  if (child_id == 11) {
    exit();
  }

  int fd[2];
  pipe(fd);
  int child_pid = fork();
  if (child_pid == 0) {
    close(fd[0]);
    int rec;
    while (read(parend_fd[0], &rec, sizeof(int)) > 0) {
      if (rec == primes[child_id]) {
        printf("prime %d\n", rec);
      } else if (rec % primes[child_id] != 0) {
        write(fd[1], &rec, sizeof(int));
      }
    }
  } else {
    close(fd[1]);
    fork_children(child_id + 1, fd);
  }
}

int main(int argc, char *argv[]) {
  int main_fd[2];
  pipe(main_fd);
  int child_id = 0;
  int child_pid = fork();

  if (child_pid == 0) {
    close(main_fd[0]);
    for (int i = 2; i < 35; ++i) {
      write(main_fd[1], &i, sizeof(int));
    }
  } else {
    close(main_fd[1]);
    fork_children(child_id, main_fd);
  }

  exit();
}
