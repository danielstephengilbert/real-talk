
#include <stdio.h>
#include <stdlib.h>

#include "../../include/help/print_usage.h"
#include "../../include/regex/validate_regex.h"

void validate_mode(char *mode) {

  char *mode_regex = "^-[sc]$";

  int valid_mode = validate_regex(mode_regex, mode);
  
  if (!valid_mode) {
    printf("\nYou must specify either server -s or client -c mode.\n");
    print_usage();
    exit(1);
  }

}

