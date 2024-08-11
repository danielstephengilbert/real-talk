#!/usr/bin/env bash

set -x

gcc \
  ./src/rt.c \
  ./src/get_listener.c \
  -o rt
