#!/usr/bin/env bash

set -x

gcc \
  ./src/rt.c \
  ./src/help.c \
  ./src/regex.c \
  ./src/validation.c \
  -o rt
