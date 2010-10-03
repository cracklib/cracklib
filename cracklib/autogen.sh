#!/bin/sh -x
autopoint -f
cd m4
echo EXTRA_DIST = *.m4 > Makefile.am
cd ..
autoreconf -f -i
