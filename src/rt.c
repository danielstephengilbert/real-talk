
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void validateArgCount(int argc);
void printUsage();
regex_t compileRegex(char *raw_regex);
int executeRegex(regex_t compiled_regex, char *pattern);
int validateRegex(char *raw_regex, char *pattern);
void validateMode(char *mode);
void validateIP(char *ip);
void validatePort(char *port);

int main(int argc, char *argv[]) {

  validateArgCount(argc);

  char *mode = argv[1];
  char *ip   = argv[2];
  char *port = argv[3];

  validateMode(mode);
  validateIP(ip);
  validatePort(port);

  return 0;

}

void validateArgCount(int argc) {

  if (argc != 4) {
    printUsage();
    exit(1);
  }

}

void printUsage() {

  printf("\nUsage:\n");
  printf("server: rt -s server_ip server_port\n");
  printf("client: rt -c server_ip server_port\n\n");

}

regex_t compileRegex(char *raw_regex) {

  regex_t compiled_regex;

  int regex_status  = regcomp(&compiled_regex, raw_regex, REG_EXTENDED);
  
  if (regex_status != 0) {
    printf("Regex failed to compile.\n");
    exit(1);
  }

  return compiled_regex;

}

int executeRegex(regex_t compiled_regex, char *pattern) {

  int regex_status = regexec(&compiled_regex, pattern, 0, NULL, 0);

  if (regex_status == 0) {
    return 1;
  } else if (regex_status == REG_NOMATCH) {
    return 0;
  } else {
    printf("Error processing regex.\n");
    exit(1);
  }

}

int validateRegex(char *raw_regex, char *pattern) {

  regex_t compiled_regex  = compileRegex(raw_regex);
  int regex_matches       = executeRegex(compiled_regex, pattern);

  if (regex_matches) {
    return 1;
  } else {
    return 0;
  }

}

void validateMode(char *mode) {

  char *mode_regex = "^-[sc]$";

  int valid_mode = validateRegex(mode_regex, mode);
  
  if (!valid_mode) {
    printf("\nYou must specify either server -s or client -c mode.\n");
    printUsage();
    exit(1);
  }

}

void validateIP(char *ip) {

  // Note: This regex is not precise. It's simply a general safeguard.
  char *ip_regex = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

  int valid_ip = validateRegex(ip_regex, ip);
  
  if (!valid_ip) {
    printf("\nYou must specify a valid IP address format: XXX.XXX.XXX.XXX.\n");
    printUsage();
    exit(1);
  }

}

void validatePort(char *port) {

  // Note: This regex is not precise. It's simply a general safeguard.
  char *port_regex = "^[0-9]{1,5}$";

  int valid_port = validateRegex(port_regex, port);
  
  if (!valid_port) {
    printf("\nYou must specify a valid port format: XXXXX.\n");
    printUsage();
    exit(1);
  }

}

