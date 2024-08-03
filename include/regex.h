#ifndef REGEX_H_
#define REGEX_H_

#include <regex.h>

regex_t compile_regex(char *raw_regex);
int execute_regex(regex_t compiled_regex, char *pattern);
int validate_regex(char *raw_regex, char *pattern);

#endif
