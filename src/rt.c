#include <stdio.h>
#include <stdlib.h>

#include "../include/socket.h"
#include "../include/validation.h"

int main(int argc, char *argv[]) {

  int listener = 0;

  validate_arg_count(argc);

  listener = get_listener_socket();
  if (listener == -1) {
    fprintf(stderr, "Error: Could not get listening socket\n");
    exit(1);
  }

  return 0;

}
