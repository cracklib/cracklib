/*
 * This program is copyright Alec Muffett 1993, portions copyright other authors.
 * The authors disclaim all responsibility or liability with respect to it's usage
 * or its effect upon hardware or computer systems.
 */

#include "config.h"
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include "packer.h"

static const char vers_id[] = "stringlib.c : v2.3p2 Alec Muffett 18 May 1993";

char
Chop(string)
    register char *string;
{
    register char c;
    register char *ptr;
    c = '\0';

    for (ptr = string; *ptr; ptr++);
    if (ptr != string)
    {
	c = *(--ptr);
	*ptr = '\0';
    }
    return (c);
}

char *
Trim(string)
    register char *string;
{
    register char *ptr;
    for (ptr = string; *ptr; ptr++);

    while ((--ptr >= string) && isspace(*ptr));

    *(++ptr) = '\0';

    return (ptr);
}

char *
Clone(string)
    char *string;
{
    register char *retval;
    retval = (char *) malloc(strlen(string) + 1);
    if (retval)
    {
	strcpy(retval, string);
    }
    return (retval);
}
