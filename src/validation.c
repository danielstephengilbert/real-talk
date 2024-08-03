#include <stdio.h>
#include <stdlib.h>

#include "../../include/help.h"
#include "../../include/regex.h"

void validate_arg_count(int argc) {

  if (argc != 4) {
    print_usage();
    exit(1);
  }

}

void validate_ip(char *ip) {

  // Note: This regex is not precise. It's simply a general safeguard.
  char *ip_regex = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

  int valid_ip = validate_regex(ip_regex, ip);
  
  if (!valid_ip) {
    printf("\nYou must specify a valid IP address format: XXX.XXX.XXX.XXX.\n");
    print_usage();
    exit(1);
  }

}

void validate_mode(char *mode) {

  char *mode_regex = "^-[sc]$";

  int valid_mode = validate_regex(mode_regex, mode);
  
  if (!valid_mode) {
    printf("\nYou must specify either server -s or client -c mode.\n");
    print_usage();
    exit(1);
  }

}

void validate_port(char *port) {

  // Note: This regex is not precise. It's simply a general safeguard.
  char *port_regex = "^[0-9]{1,5}$";

  int valid_port = validate_regex(port_regex, port);
  
  if (!valid_port) {
    printf("\nYou must specify a valid port format: XXXXX.\n");
    print_usage();
    exit(1);
  }

}
