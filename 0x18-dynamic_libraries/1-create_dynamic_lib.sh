#!/bin/bash
gcc -fPIC -C *.C
gcc -shared *.O  -O liball.SO
