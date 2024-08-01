#include <stdio.h>
#include <stdlib.h>

#include "../../include/help/print_usage.h"
#include "../../include/regex/validate_regex.h"

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
