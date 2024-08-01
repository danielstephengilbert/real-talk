
#include <stdlib.h>

#include "../../include/help/print_usage.h"

void validate_arg_count(int argc) {

  if (argc != 4) {
    print_usage();
    exit(1);
  }

}

