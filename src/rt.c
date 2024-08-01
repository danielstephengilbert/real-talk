#include "../include/validation/validate_arg_count.h"
#include "../include/validation/validate_ip.h"
#include "../include/validation/validate_mode.h"
#include "../include/validation/validate_port.h"

int main(int argc, char *argv[]) {

  validate_arg_count(argc);

  char *mode = argv[1];
  char *ip   = argv[2];
  char *port = argv[3];

  validate_mode(mode);
  validate_ip(ip);
  validate_port(port);

  return 0;

}
