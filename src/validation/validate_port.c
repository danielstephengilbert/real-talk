
#include <stdio.h>
#include <stdlib.h>

#include "../../include/help/print_usage.h"
#include "../../include/regex/validate_regex.h"

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

