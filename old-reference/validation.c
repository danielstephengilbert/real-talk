#include <stdio.h>
#include <stdlib.h>

#include "../include/help.h"
#include "../include/regex.h"

void validate_arg_count(int argc) {

  // The name of the program is passed as the first argument
  // and is not user-supplied
  int real_arg_count = argc - 1;

  if (real_arg_count != 0 && real_arg_count != 2) {
    fprintf(stderr, "\nYou must supply either 0 or 2 arguments,\n");
    fprintf(stderr, "depending on the mode.\nSee usage.\n");
    print_usage();
    exit(1);
  }

}

void validate_ip(char *ip) {

  // Note: This regex is not precise. It's simply a general safeguard.
  char *ip_regex = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

  int valid_ip = validate_regex(ip_regex, ip);
  
  if (!valid_ip) {
    fprintf(stderr, "\nYou must specify a valid IP address format: XXX.XXX.XXX.XXX.\n");
    print_usage();
    exit(1);
  }

}

void validate_mode(char *mode) {

  char *mode_regex = "^-[sc]$";

  int valid_mode = validate_regex(mode_regex, mode);
  
  if (!valid_mode) {
    fprintf(stderr, "\nYou must specify either server -s or client -c mode.\n");
    print_usage();
    exit(1);
  }

}

void validate_port(char *port) {

  // Note: This regex is not precise. It's simply a general safeguard.
  char *port_regex = "^[0-9]{1,5}$";

  int valid_port = validate_regex(port_regex, port);
  
  if (!valid_port) {
    fprintf(stderr, "\nYou must specify a valid port format: XXXXX.\n");
    print_usage();
    exit(1);
  }

}
