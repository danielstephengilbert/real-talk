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

int validate_regex(char *raw_regex, char *pattern) {

  regex_t compiled_regex  = compile_regex(raw_regex);
  int regex_matches       = execute_regex(compiled_regex, pattern);

  if (regex_matches) {
    return 1;
  } else {
    return 0;
  }

}
