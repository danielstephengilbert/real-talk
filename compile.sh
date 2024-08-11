#!/usr/bin/env bash

set -x

gcc \
  ./src/rt.c \
  ./src/add_connection.c \
  ./src/get_ipv46_addr.c \
  ./src/get_listener.c \
  -o rt
