#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

regex_t compile_regex(char *raw_regex) {

  regex_t compiled_regex;

  int regex_status = regcomp(&compiled_regex, raw_regex, REG_EXTENDED);
  
  if (regex_status != 0) {
    printf("\nRegex failed to compile.\n");
    exit(1);
  }

  return compiled_regex;

}
