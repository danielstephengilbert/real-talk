#!/usr/bin/env bash

# compile & run code
# can be used to test

set -x

gcc ./src/rt.c -o rt
./rt
./rt -s 127.0.0.1 7001
./rt -c 127.0.0.1 7001
./rt -x 127.0.0.1 7001

