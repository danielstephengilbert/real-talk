#include <regex.h>

#include "../../include/regex/compile_regex.h"
#include "../../include/regex/execute_regex.h"

int validate_regex(char *raw_regex, char *pattern) {

  regex_t compiled_regex  = compile_regex(raw_regex);
  int regex_matches       = execute_regex(compiled_regex, pattern);

  if (regex_matches) {
    return 1;
  } else {
    return 0;
  }

}
