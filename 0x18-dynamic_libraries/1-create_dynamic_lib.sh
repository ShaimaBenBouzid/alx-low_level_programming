#!/bin/bash
gcc -fPIC -C *.C
gcc -shared *.O liball.SO
