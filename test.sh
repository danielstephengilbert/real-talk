
#!/usr/bin/env bash

set -x

./compile.sh

./rt -s 127.0.0.1 7001

