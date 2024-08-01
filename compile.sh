#!/usr/bin/env bash

set -x

gcc \
  ./src/rt.c \
  ./src/help/print_usage.c \
  ./src/regex/compile_regex.c \
  ./src/regex/execute_regex.c \
  ./src/regex/validate_regex.c \
  ./src/validation/validate_arg_count.c \
  ./src/validation/validate_ip.c \
  ./src/validation/validate_mode.c \
  ./src/validation/validate_port.c \
  -o rt
