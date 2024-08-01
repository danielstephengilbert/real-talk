#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

int execute_regex(regex_t compiled_regex, char *pattern) {

  int regex_status = regexec(&compiled_regex, pattern, 0, NULL, 0);

  if (regex_status == 0) {
    return 1;
  } else if (regex_status == REG_NOMATCH) {
    return 0;
  } else {
    printf("\nError processing regex.\n");
    exit(1);
  }

}
