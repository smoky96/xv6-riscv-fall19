//
// Created by dong on 2020/3/5.
//
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("give me a number please.\n");
    exit();
  }
  int time = atoi(argv[1]);
  sleep(time);

  exit();
}
