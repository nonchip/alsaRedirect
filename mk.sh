#!/bin/sh
gcc -g -fPIC -shared -Wl,-soname,alsa_redirect.so -ldl -o alsa_redirect.so alsa_redirect.c

