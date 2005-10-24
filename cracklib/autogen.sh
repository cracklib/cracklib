#!/bin/sh -x

# This script runs commands necessary to generate a Makefile for libgif.

#echo "Warning: This script will run configure for you -- if you need to pass"
#echo "  arguments to configure, please give them as arguments to this script."

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

THEDIR="`pwd`"
cd $srcdir

aclocal -I m4 || exit 1
autoheader || exit 1
libtoolize --automake --copy || exit 1
automake --add-missing --copy || exit 1
autoconf || exit 1
automake || exit 1

cd $THEDIR

# I hate that... 
#$srcdir/configure $*

exit 0

