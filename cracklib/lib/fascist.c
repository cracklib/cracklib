/*
 * This program is copyright Alec Muffett 1993. The author disclaims all
 * responsibility or liability with respect to it's usage or its effect
 * upon hardware or computer systems, and maintains copyright as set out
 * in the "LICENCE" document which accompanies distributions of Crack v4.0
 * and upwards.
 */

static char vers_id[] = "fascist.c : v2.3p3 Alec Muffett 14 dec 1997";

#include "config.h"
#include "packer.h"
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#define ISSKIP(x) (isspace(x) || ispunct(x))

#define MINDIFF 5
#define MINLEN 6
#define MAXSTEP 4

#undef DEBUG
#undef DEBUG2

extern char *Reverse(char *buf);
extern char *Lowercase(char *buf);

static char *r_destructors[] = {
    ":",                        /* noop - must do this to test raw word. */

#ifdef DEBUG2
    (char *) 0,
#endif

    "[",                        /* trimming leading/trailing junk */
    "]",
    "[[",
    "]]",
    "[[[",
    "]]]",

    "/?p@?p",                   /* purging out punctuation/symbols/junk */
    "/?s@?s",
    "/?X@?X",

    /* attempt reverse engineering of password strings */

    "/$s$s",
    "/$s$s/0s0o",
    "/$s$s/0s0o/2s2a",
    "/$s$s/0s0o/2s2a/3s3e",
    "/$s$s/0s0o/2s2a/3s3e/5s5s",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/1s1i",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/1s1l",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/1s1i/4s4a",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/1s1i/4s4h",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/1s1l/4s4a",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/1s1l/4s4h",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/4s4a",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/4s4h",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/4s4a",
    "/$s$s/0s0o/2s2a/3s3e/5s5s/4s4h",
    "/$s$s/0s0o/2s2a/3s3e/1s1i",
    "/$s$s/0s0o/2s2a/3s3e/1s1l",
    "/$s$s/0s0o/2s2a/3s3e/1s1i/4s4a",
    "/$s$s/0s0o/2s2a/3s3e/1s1i/4s4h",
    "/$s$s/0s0o/2s2a/3s3e/1s1l/4s4a",
    "/$s$s/0s0o/2s2a/3s3e/1s1l/4s4h",
    "/$s$s/0s0o/2s2a/3s3e/4s4a",
    "/$s$s/0s0o/2s2a/3s3e/4s4h",
    "/$s$s/0s0o/2s2a/3s3e/4s4a",
    "/$s$s/0s0o/2s2a/3s3e/4s4h",
    "/$s$s/0s0o/2s2a/5s5s",
    "/$s$s/0s0o/2s2a/5s5s/1s1i",
    "/$s$s/0s0o/2s2a/5s5s/1s1l",
    "/$s$s/0s0o/2s2a/5s5s/1s1i/4s4a",
    "/$s$s/0s0o/2s2a/5s5s/1s1i/4s4h",
    "/$s$s/0s0o/2s2a/5s5s/1s1l/4s4a",
    "/$s$s/0s0o/2s2a/5s5s/1s1l/4s4h",
    "/$s$s/0s0o/2s2a/5s5s/4s4a",
    "/$s$s/0s0o/2s2a/5s5s/4s4h",
    "/$s$s/0s0o/2s2a/5s5s/4s4a",
    "/$s$s/0s0o/2s2a/5s5s/4s4h",
    "/$s$s/0s0o/2s2a/1s1i",
    "/$s$s/0s0o/2s2a/1s1l",
    "/$s$s/0s0o/2s2a/1s1i/4s4a",
    "/$s$s/0s0o/2s2a/1s1i/4s4h",
    "/$s$s/0s0o/2s2a/1s1l/4s4a",
    "/$s$s/0s0o/2s2a/1s1l/4s4h",
    "/$s$s/0s0o/2s2a/4s4a",
    "/$s$s/0s0o/2s2a/4s4h",
    "/$s$s/0s0o/2s2a/4s4a",
    "/$s$s/0s0o/2s2a/4s4h",
    "/$s$s/0s0o/3s3e",
    "/$s$s/0s0o/3s3e/5s5s",
    "/$s$s/0s0o/3s3e/5s5s/1s1i",
    "/$s$s/0s0o/3s3e/5s5s/1s1l",
    "/$s$s/0s0o/3s3e/5s5s/1s1i/4s4a",
    "/$s$s/0s0o/3s3e/5s5s/1s1i/4s4h",
    "/$s$s/0s0o/3s3e/5s5s/1s1l/4s4a",
    "/$s$s/0s0o/3s3e/5s5s/1s1l/4s4h",
    "/$s$s/0s0o/3s3e/5s5s/4s4a",
    "/$s$s/0s0o/3s3e/5s5s/4s4h",
    "/$s$s/0s0o/3s3e/5s5s/4s4a",
    "/$s$s/0s0o/3s3e/5s5s/4s4h",
    "/$s$s/0s0o/3s3e/1s1i",
    "/$s$s/0s0o/3s3e/1s1l",
    "/$s$s/0s0o/3s3e/1s1i/4s4a",
    "/$s$s/0s0o/3s3e/1s1i/4s4h",
    "/$s$s/0s0o/3s3e/1s1l/4s4a",
    "/$s$s/0s0o/3s3e/1s1l/4s4h",
    "/$s$s/0s0o/3s3e/4s4a",
    "/$s$s/0s0o/3s3e/4s4h",
    "/$s$s/0s0o/3s3e/4s4a",
    "/$s$s/0s0o/3s3e/4s4h",
    "/$s$s/0s0o/5s5s",
    "/$s$s/0s0o/5s5s/1s1i",
    "/$s$s/0s0o/5s5s/1s1l",
    "/$s$s/0s0o/5s5s/1s1i/4s4a",
    "/$s$s/0s0o/5s5s/1s1i/4s4h",
    "/$s$s/0s0o/5s5s/1s1l/4s4a",
    "/$s$s/0s0o/5s5s/1s1l/4s4h",
    "/$s$s/0s0o/5s5s/4s4a",
    "/$s$s/0s0o/5s5s/4s4h",
    "/$s$s/0s0o/5s5s/4s4a",
    "/$s$s/0s0o/5s5s/4s4h",
    "/$s$s/0s0o/1s1i",
    "/$s$s/0s0o/1s1l",
    "/$s$s/0s0o/1s1i/4s4a",
    "/$s$s/0s0o/1s1i/4s4h",
    "/$s$s/0s0o/1s1l/4s4a",
    "/$s$s/0s0o/1s1l/4s4h",
    "/$s$s/0s0o/4s4a",
    "/$s$s/0s0o/4s4h",
    "/$s$s/0s0o/4s4a",
    "/$s$s/0s0o/4s4h",
    "/$s$s/2s2a",
    "/$s$s/2s2a/3s3e",
    "/$s$s/2s2a/3s3e/5s5s",
    "/$s$s/2s2a/3s3e/5s5s/1s1i",
    "/$s$s/2s2a/3s3e/5s5s/1s1l",
    "/$s$s/2s2a/3s3e/5s5s/1s1i/4s4a",
    "/$s$s/2s2a/3s3e/5s5s/1s1i/4s4h",
    "/$s$s/2s2a/3s3e/5s5s/1s1l/4s4a",
    "/$s$s/2s2a/3s3e/5s5s/1s1l/4s4h",
    "/$s$s/2s2a/3s3e/5s5s/4s4a",
    "/$s$s/2s2a/3s3e/5s5s/4s4h",
    "/$s$s/2s2a/3s3e/5s5s/4s4a",
    "/$s$s/2s2a/3s3e/5s5s/4s4h",
    "/$s$s/2s2a/3s3e/1s1i",
    "/$s$s/2s2a/3s3e/1s1l",
    "/$s$s/2s2a/3s3e/1s1i/4s4a",
    "/$s$s/2s2a/3s3e/1s1i/4s4h",
    "/$s$s/2s2a/3s3e/1s1l/4s4a",
    "/$s$s/2s2a/3s3e/1s1l/4s4h",
    "/$s$s/2s2a/3s3e/4s4a",
    "/$s$s/2s2a/3s3e/4s4h",
    "/$s$s/2s2a/3s3e/4s4a",
    "/$s$s/2s2a/3s3e/4s4h",
    "/$s$s/2s2a/5s5s",
    "/$s$s/2s2a/5s5s/1s1i",
    "/$s$s/2s2a/5s5s/1s1l",
    "/$s$s/2s2a/5s5s/1s1i/4s4a",
    "/$s$s/2s2a/5s5s/1s1i/4s4h",
    "/$s$s/2s2a/5s5s/1s1l/4s4a",
    "/$s$s/2s2a/5s5s/1s1l/4s4h",
    "/$s$s/2s2a/5s5s/4s4a",
    "/$s$s/2s2a/5s5s/4s4h",
    "/$s$s/2s2a/5s5s/4s4a",
    "/$s$s/2s2a/5s5s/4s4h",
    "/$s$s/2s2a/1s1i",
    "/$s$s/2s2a/1s1l",
    "/$s$s/2s2a/1s1i/4s4a",
    "/$s$s/2s2a/1s1i/4s4h",
    "/$s$s/2s2a/1s1l/4s4a",
    "/$s$s/2s2a/1s1l/4s4h",
    "/$s$s/2s2a/4s4a",
    "/$s$s/2s2a/4s4h",
    "/$s$s/2s2a/4s4a",
    "/$s$s/2s2a/4s4h",
    "/$s$s/3s3e",
    "/$s$s/3s3e/5s5s",
    "/$s$s/3s3e/5s5s/1s1i",
    "/$s$s/3s3e/5s5s/1s1l",
    "/$s$s/3s3e/5s5s/1s1i/4s4a",
    "/$s$s/3s3e/5s5s/1s1i/4s4h",
    "/$s$s/3s3e/5s5s/1s1l/4s4a",
    "/$s$s/3s3e/5s5s/1s1l/4s4h",
    "/$s$s/3s3e/5s5s/4s4a",
    "/$s$s/3s3e/5s5s/4s4h",
    "/$s$s/3s3e/5s5s/4s4a",
    "/$s$s/3s3e/5s5s/4s4h",
    "/$s$s/3s3e/1s1i",
    "/$s$s/3s3e/1s1l",
    "/$s$s/3s3e/1s1i/4s4a",
    "/$s$s/3s3e/1s1i/4s4h",
    "/$s$s/3s3e/1s1l/4s4a",
    "/$s$s/3s3e/1s1l/4s4h",
    "/$s$s/3s3e/4s4a",
    "/$s$s/3s3e/4s4h",
    "/$s$s/3s3e/4s4a",
    "/$s$s/3s3e/4s4h",
    "/$s$s/5s5s",
    "/$s$s/5s5s/1s1i",
    "/$s$s/5s5s/1s1l",
    "/$s$s/5s5s/1s1i/4s4a",
    "/$s$s/5s5s/1s1i/4s4h",
    "/$s$s/5s5s/1s1l/4s4a",
    "/$s$s/5s5s/1s1l/4s4h",
    "/$s$s/5s5s/4s4a",
    "/$s$s/5s5s/4s4h",
    "/$s$s/5s5s/4s4a",
    "/$s$s/5s5s/4s4h",
    "/$s$s/1s1i",
    "/$s$s/1s1l",
    "/$s$s/1s1i/4s4a",
    "/$s$s/1s1i/4s4h",
    "/$s$s/1s1l/4s4a",
    "/$s$s/1s1l/4s4h",
    "/$s$s/4s4a",
    "/$s$s/4s4h",
    "/$s$s/4s4a",
    "/$s$s/4s4h",
    "/0s0o",
    "/0s0o/2s2a",
    "/0s0o/2s2a/3s3e",
    "/0s0o/2s2a/3s3e/5s5s",
    "/0s0o/2s2a/3s3e/5s5s/1s1i",
    "/0s0o/2s2a/3s3e/5s5s/1s1l",
    "/0s0o/2s2a/3s3e/5s5s/1s1i/4s4a",
    "/0s0o/2s2a/3s3e/5s5s/1s1i/4s4h",
    "/0s0o/2s2a/3s3e/5s5s/1s1l/4s4a",
    "/0s0o/2s2a/3s3e/5s5s/1s1l/4s4h",
    "/0s0o/2s2a/3s3e/5s5s/4s4a",
    "/0s0o/2s2a/3s3e/5s5s/4s4h",
    "/0s0o/2s2a/3s3e/5s5s/4s4a",
    "/0s0o/2s2a/3s3e/5s5s/4s4h",
    "/0s0o/2s2a/3s3e/1s1i",
    "/0s0o/2s2a/3s3e/1s1l",
    "/0s0o/2s2a/3s3e/1s1i/4s4a",
    "/0s0o/2s2a/3s3e/1s1i/4s4h",
    "/0s0o/2s2a/3s3e/1s1l/4s4a",
    "/0s0o/2s2a/3s3e/1s1l/4s4h",
    "/0s0o/2s2a/3s3e/4s4a",
    "/0s0o/2s2a/3s3e/4s4h",
    "/0s0o/2s2a/3s3e/4s4a",
    "/0s0o/2s2a/3s3e/4s4h",
    "/0s0o/2s2a/5s5s",
    "/0s0o/2s2a/5s5s/1s1i",
    "/0s0o/2s2a/5s5s/1s1l",
    "/0s0o/2s2a/5s5s/1s1i/4s4a",
    "/0s0o/2s2a/5s5s/1s1i/4s4h",
    "/0s0o/2s2a/5s5s/1s1l/4s4a",
    "/0s0o/2s2a/5s5s/1s1l/4s4h",
    "/0s0o/2s2a/5s5s/4s4a",
    "/0s0o/2s2a/5s5s/4s4h",
    "/0s0o/2s2a/5s5s/4s4a",
    "/0s0o/2s2a/5s5s/4s4h",
    "/0s0o/2s2a/1s1i",
    "/0s0o/2s2a/1s1l",
    "/0s0o/2s2a/1s1i/4s4a",
    "/0s0o/2s2a/1s1i/4s4h",
    "/0s0o/2s2a/1s1l/4s4a",
    "/0s0o/2s2a/1s1l/4s4h",
    "/0s0o/2s2a/4s4a",
    "/0s0o/2s2a/4s4h",
    "/0s0o/2s2a/4s4a",
    "/0s0o/2s2a/4s4h",
    "/0s0o/3s3e",
    "/0s0o/3s3e/5s5s",
    "/0s0o/3s3e/5s5s/1s1i",
    "/0s0o/3s3e/5s5s/1s1l",
    "/0s0o/3s3e/5s5s/1s1i/4s4a",
    "/0s0o/3s3e/5s5s/1s1i/4s4h",
    "/0s0o/3s3e/5s5s/1s1l/4s4a",
    "/0s0o/3s3e/5s5s/1s1l/4s4h",
    "/0s0o/3s3e/5s5s/4s4a",
    "/0s0o/3s3e/5s5s/4s4h",
    "/0s0o/3s3e/5s5s/4s4a",
    "/0s0o/3s3e/5s5s/4s4h",
    "/0s0o/3s3e/1s1i",
    "/0s0o/3s3e/1s1l",
    "/0s0o/3s3e/1s1i/4s4a",
    "/0s0o/3s3e/1s1i/4s4h",
    "/0s0o/3s3e/1s1l/4s4a",
    "/0s0o/3s3e/1s1l/4s4h",
    "/0s0o/3s3e/4s4a",
    "/0s0o/3s3e/4s4h",
    "/0s0o/3s3e/4s4a",
    "/0s0o/3s3e/4s4h",
    "/0s0o/5s5s",
    "/0s0o/5s5s/1s1i",
    "/0s0o/5s5s/1s1l",
    "/0s0o/5s5s/1s1i/4s4a",
    "/0s0o/5s5s/1s1i/4s4h",
    "/0s0o/5s5s/1s1l/4s4a",
    "/0s0o/5s5s/1s1l/4s4h",
    "/0s0o/5s5s/4s4a",
    "/0s0o/5s5s/4s4h",
    "/0s0o/5s5s/4s4a",
    "/0s0o/5s5s/4s4h",
    "/0s0o/1s1i",
    "/0s0o/1s1l",
    "/0s0o/1s1i/4s4a",
    "/0s0o/1s1i/4s4h",
    "/0s0o/1s1l/4s4a",
    "/0s0o/1s1l/4s4h",
    "/0s0o/4s4a",
    "/0s0o/4s4h",
    "/0s0o/4s4a",
    "/0s0o/4s4h",
    "/2s2a",
    "/2s2a/3s3e",
    "/2s2a/3s3e/5s5s",
    "/2s2a/3s3e/5s5s/1s1i",
    "/2s2a/3s3e/5s5s/1s1l",
    "/2s2a/3s3e/5s5s/1s1i/4s4a",
    "/2s2a/3s3e/5s5s/1s1i/4s4h",
    "/2s2a/3s3e/5s5s/1s1l/4s4a",
    "/2s2a/3s3e/5s5s/1s1l/4s4h",
    "/2s2a/3s3e/5s5s/4s4a",
    "/2s2a/3s3e/5s5s/4s4h",
    "/2s2a/3s3e/5s5s/4s4a",
    "/2s2a/3s3e/5s5s/4s4h",
    "/2s2a/3s3e/1s1i",
    "/2s2a/3s3e/1s1l",
    "/2s2a/3s3e/1s1i/4s4a",
    "/2s2a/3s3e/1s1i/4s4h",
    "/2s2a/3s3e/1s1l/4s4a",
    "/2s2a/3s3e/1s1l/4s4h",
    "/2s2a/3s3e/4s4a",
    "/2s2a/3s3e/4s4h",
    "/2s2a/3s3e/4s4a",
    "/2s2a/3s3e/4s4h",
    "/2s2a/5s5s",
    "/2s2a/5s5s/1s1i",
    "/2s2a/5s5s/1s1l",
    "/2s2a/5s5s/1s1i/4s4a",
    "/2s2a/5s5s/1s1i/4s4h",
    "/2s2a/5s5s/1s1l/4s4a",
    "/2s2a/5s5s/1s1l/4s4h",
    "/2s2a/5s5s/4s4a",
    "/2s2a/5s5s/4s4h",
    "/2s2a/5s5s/4s4a",
    "/2s2a/5s5s/4s4h",
    "/2s2a/1s1i",
    "/2s2a/1s1l",
    "/2s2a/1s1i/4s4a",
    "/2s2a/1s1i/4s4h",
    "/2s2a/1s1l/4s4a",
    "/2s2a/1s1l/4s4h",
    "/2s2a/4s4a",
    "/2s2a/4s4h",
    "/2s2a/4s4a",
    "/2s2a/4s4h",
    "/3s3e",
    "/3s3e/5s5s",
    "/3s3e/5s5s/1s1i",
    "/3s3e/5s5s/1s1l",
    "/3s3e/5s5s/1s1i/4s4a",
    "/3s3e/5s5s/1s1i/4s4h",
    "/3s3e/5s5s/1s1l/4s4a",
    "/3s3e/5s5s/1s1l/4s4h",
    "/3s3e/5s5s/4s4a",
    "/3s3e/5s5s/4s4h",
    "/3s3e/5s5s/4s4a",
    "/3s3e/5s5s/4s4h",
    "/3s3e/1s1i",
    "/3s3e/1s1l",
    "/3s3e/1s1i/4s4a",
    "/3s3e/1s1i/4s4h",
    "/3s3e/1s1l/4s4a",
    "/3s3e/1s1l/4s4h",
    "/3s3e/4s4a",
    "/3s3e/4s4h",
    "/3s3e/4s4a",
    "/3s3e/4s4h",
    "/5s5s",
    "/5s5s/1s1i",
    "/5s5s/1s1l",
    "/5s5s/1s1i/4s4a",
    "/5s5s/1s1i/4s4h",
    "/5s5s/1s1l/4s4a",
    "/5s5s/1s1l/4s4h",
    "/5s5s/4s4a",
    "/5s5s/4s4h",
    "/5s5s/4s4a",
    "/5s5s/4s4h",
    "/1s1i",
    "/1s1l",
    "/1s1i/4s4a",
    "/1s1i/4s4h",
    "/1s1l/4s4a",
    "/1s1l/4s4h",
    "/4s4a",
    "/4s4h",
    "/4s4a",
    "/4s4h",

    /* done */
    (char *) 0
};

static char *r_constructors[] = {
    ":",

#ifdef DEBUG2
    (char *) 0,
#endif

    "r",
    "d",
    "f",
    "dr",
    "fr",
    "rf",
    (char *) 0
};

int
GTry(rawtext, password)
    char *rawtext;
    char *password;
{
    int i;
    int len;
    char *mp;

    /* use destructors to turn password into rawtext */
    /* note use of Reverse() to save duplicating all rules */

    len = strlen(password);

    for (i = 0; r_destructors[i]; i++)
    {
	if (!(mp = Mangle(password, r_destructors[i])))
	{
	    continue;
	}

#ifdef DEBUG
	printf("%-16s = %-16s (destruct %s)\n", mp, rawtext, r_destructors[i]);
#endif

	if (!strncmp(mp, rawtext, len))
	{
	    return (1);
	}

#ifdef DEBUG
	printf("%-16s = %-16s (destruct %s reversed)\n", Reverse(mp), rawtext, r_destructors[i]);
#endif

	if (!strncmp(Reverse(mp), rawtext, len))
	{
	    return (1);
	}
    }

    for (i = 0; r_constructors[i]; i++)
    {
	if (!(mp = Mangle(rawtext, r_constructors[i])))
	{
	    continue;
	}

#ifdef DEBUG
	printf("%-16s = %-16s (construct %s)\n", mp, password, r_constructors[i]);
#endif

	if (!strncmp(mp, password, len))
	{
	    return (1);
	}
    }

    return (0);
}

char *
FascistGecos(password, uid)
    char *password;
    int uid;
{
    int i;
    int j;
    int wc;
    char *ptr;
    int gwords;
    struct passwd *pwp, passwd;
    char gbuffer[STRINGSIZE];
    char tbuffer[STRINGSIZE];
    char *sbuffer = NULL;
#ifdef HAVE_GETPWUID_R
    size_t sbufferlen = LINE_MAX;
#endif
    char *uwords[STRINGSIZE];
    char longbuffer[STRINGSIZE * 2];

#ifdef HAVE_GETPWUID_R
    sbuffer = malloc(sbufferlen);
    if (sbuffer == NULL)
    {
        return ("memory allocation error");
    }
    while ((i = getpwuid_r(uid, &passwd, sbuffer, sbufferlen, &pwp)) != 0)
    {
        if (i == ERANGE)
        {
            free(sbuffer);

	    sbufferlen += LINE_MAX;
            sbuffer = malloc(sbufferlen);

            if (sbuffer == NULL)
            {
                return ("memory allocation error");
            }
        } else {
            pwp = NULL;
            break;
        }
    }
#else
    /* Non-reentrant, but no choice since no _r routine */
    pwp = getpwuid(uid);
#endif

    if (pwp == NULL)
    {
	if (sbuffer)
	{
		free(sbuffer);
		sbuffer = NULL;
	}
	return ("you are not registered in the password file");
    }

    /* lets get really paranoid and assume a dangerously long gecos entry */

    strncpy(tbuffer, pwp->pw_name, STRINGSIZE);
    tbuffer[STRINGSIZE-1] = '\0';
    if (GTry(tbuffer, password))
    {
	if (sbuffer)
	{
		free(sbuffer);
		sbuffer = NULL;
	}
	return ("it is based on your username");
    }

    /* it never used to be that you got passwd strings > 1024 chars, but now... */

    strncpy(tbuffer, pwp->pw_gecos, STRINGSIZE);
    tbuffer[STRINGSIZE-1] = '\0';
    strcpy(gbuffer, Lowercase(tbuffer));

    wc = 0;
    ptr = gbuffer;
    gwords = 0;
    uwords[0] = (char *)0;

    while (*ptr)
    {
	while (*ptr && ISSKIP(*ptr))
	{
	    ptr++;
	}

	if (ptr != gbuffer)
	{
	    ptr[-1] = '\0';
	}

	gwords++;
	uwords[wc++] = ptr;

	if (wc == STRINGSIZE)
	{
	    uwords[--wc] = (char *) 0;  /* to hell with it */
	    break;
	} else
	{
	    uwords[wc] = (char *) 0;
	}

	while (*ptr && !ISSKIP(*ptr))
	{
	    ptr++;
	}

	if (*ptr)
	{
	    *(ptr++) = '\0';
	}
    }

#ifdef DEBUG
    for (i = 0; uwords[i]; i++)
    {
	printf("gecosword %s\n", uwords[i]);
    }
#endif

    for (i = 0; uwords[i]; i++)
    {
	if (GTry(uwords[i], password))
	{
	    if (sbuffer)
	    {
	    	free(sbuffer);
		sbuffer = NULL;
	    }
	    return ("it is based upon your password entry");
	}
    }

    /* since uwords are taken from gbuffer, no uword can be longer than gbuffer */

    for (j = 1; (j < gwords) && uwords[j]; j++)
    {
	for (i = 0; i < j; i++)
	{
	    strcpy(longbuffer, uwords[i]);
	    strcat(longbuffer, uwords[j]);

	    if (GTry(longbuffer, password))
	    {
	        if (sbuffer)
	        {
	       	    free(sbuffer);
		    sbuffer = NULL;
	        }
		return ("it is derived from your password entry");
	    }

	    strcpy(longbuffer, uwords[j]);
	    strcat(longbuffer, uwords[i]);

	    if (GTry(longbuffer, password))
	    {
	        if (sbuffer)
	        {
	       	    free(sbuffer);
		    sbuffer = NULL;
	        }
		return ("it's derived from your password entry");
	    }

	    longbuffer[0] = uwords[i][0];
	    longbuffer[1] = '\0';
	    strcat(longbuffer, uwords[j]);

	    if (GTry(longbuffer, password))
	    {
	        if (sbuffer)
	        {
	       	    free(sbuffer);
		    sbuffer = NULL;
	        }
		return ("it is derivable from your password entry");
	    }

	    longbuffer[0] = uwords[j][0];
	    longbuffer[1] = '\0';
	    strcat(longbuffer, uwords[i]);

	    if (GTry(longbuffer, password))
	    {
	        if (sbuffer)
	        {
	       	    free(sbuffer);
		    sbuffer = NULL;
	        }
		return ("it's derivable from your password entry");
	    }
	}
    }

    if (sbuffer)
    {
        free(sbuffer);
        sbuffer = NULL;
    }

    return ((char *) 0);
}

char *
FascistLook(pwp, instring)
    PWDICT *pwp;
    char *instring;
{
    int i;
    char *ptr;
    char *jptr;
    char junk[STRINGSIZE];
    char *password;
    char rpassword[STRINGSIZE];
    int32 notfound;

    notfound = PW_WORDS(pwp);
    /* already truncated if from FascistCheck() */
    /* but pretend it wasn't ... */
    strncpy(rpassword, instring, TRUNCSTRINGSIZE);
    rpassword[TRUNCSTRINGSIZE - 1] = '\0';
    password = rpassword;

    if (strlen(password) < 4)
    {
	return ("it's WAY too short");
    }

    if (strlen(password) < MINLEN)
    {
	return ("it is too short");
    }

    jptr = junk;
    *jptr = '\0';

    for (i = 0; i < STRINGSIZE && password[i]; i++)
    {
	if (!strchr(junk, password[i]))
	{
	    *(jptr++) = password[i];
	    *jptr = '\0';
	}
    }

    if (strlen(junk) < MINDIFF)
    {
	return ("it does not contain enough DIFFERENT characters");
    }

    strcpy(password, (char *)Lowercase(password));

    Trim(password);

    while (*password && isspace(*password))
    {
	password++;
    }

    if (!*password)
    {
	return ("it is all whitespace");
    }

    i = 0;
    ptr = password;
    while (ptr[0] && ptr[1])
    {
	if ((ptr[1] == (ptr[0] + 1)) || (ptr[1] == (ptr[0] - 1)))
	{
	    i++;
	}
	ptr++;
    }

    if (i > MAXSTEP)
    {
	return ("it is too simplistic/systematic");
    }

    if (PMatch("aadddddda", password))  /* smirk */
    {
	return ("it looks like a National Insurance number.");
    }

    if (ptr = FascistGecos(password, getuid()))
    {
	return (ptr);
    }

    /* it should be safe to use Mangle with its reliance on STRINGSIZE
       since password cannot be longer than TRUNCSTRINGSIZE;
       nonetheless this is not an elegant solution */

    for (i = 0; r_destructors[i]; i++)
    {
	char *a;

	if (!(a = Mangle(password, r_destructors[i])))
	{
	    continue;
	}

#ifdef DEBUG
	printf("%-16s (dict)\n", a);
#endif

	if (FindPW(pwp, a) != notfound)
	{
	    return ("it is based on a dictionary word");
	}
    }

    strcpy(password, (char *)Reverse(password));

    for (i = 0; r_destructors[i]; i++)
    {
	char *a;

	if (!(a = Mangle(password, r_destructors[i])))
	{
	    continue;
	}
#ifdef DEBUG
	printf("%-16s (reversed dict)\n", a);
#endif
	if (FindPW(pwp, a) != notfound)
	{
	    return ("it is based on a (reversed) dictionary word");
	}
    }

    return ((char *) 0);
}

char *
FascistCheck(password, path)
    char *password;
    char *path;
{
    PWDICT *pwp;
    char pwtrunced[STRINGSIZE];
    char *res;

    /* If passed null for the path, use a compiled-in default */
    if ( ! path )
    {
	path = DEFAULT_CRACKLIB_DICT;
    }

    /* security problem: assume we may have been given a really long
       password (buffer attack) and so truncate it to a workable size;
       try to define workable size as something from which we cannot
       extend a buffer beyond its limits in the rest of the code */

    strncpy(pwtrunced, password, TRUNCSTRINGSIZE);
    pwtrunced[TRUNCSTRINGSIZE - 1] = '\0'; /* enforce */

    /* perhaps someone should put something here to check if password
       is really long and syslog() a message denoting buffer attacks?  */

    if (!(pwp = PWOpen(path, "r")))
    {
        /* shouldn't perror in a library or exit */
	/* but should we return a "bad password" or "good password" if this error occurs */
	perror("PWOpen");
	exit(-1);
    }

    /* sure seems like we should close the database, since we're only likely to check one password */
    res = FascistLook(pwp, pwtrunced);

    PWClose(pwp);
    pwp = (PWDICT *)0;

    return res;
}
