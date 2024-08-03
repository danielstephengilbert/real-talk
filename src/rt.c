#include <stdlib.h>
#include <sys/socket.h>

#include "../include/socket.h"
#include "../include/validation.h"

int main(int argc, char *argv[]) {

  validate_arg_count(argc);

  char *_mode = argv[1];
  char *_ip   = argv[2];
  char *_port = argv[3];

  validate_mode(_mode);
  validate_ip(_ip);
  validate_port(_port);

  int listening_socket = get_listening_socket(_port);
  if (listening_socket == -1) exit(1);

  return 0;

}
