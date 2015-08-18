/*
 * This program is copyright Alec Muffett 1993, portions copyright other authors.
 * The authors disclaim all responsibility or liability with respect to it's usage
 * or its effect upon hardware or computer systems.
 */

#include <stdio.h>
#include <stdlib.h>

#define IN_CRACKLIB

#include "config.h"
#include "crack.h"
#include "packer.h"

int
main ()
{
    uint32_t i;
    PWDICT *pwp;
    char buffer[STRINGSIZE];

    if (!(pwp = PWOpen (NULL, "r")))
    {
	perror ("PWOpen");
	return (-1);
    }

    printf("enter dictionary word numbers, one per line...\n");

    while (fgets (buffer, STRINGSIZE, stdin))
    {
	char *c;

	sscanf (buffer, "%u", &i);

	printf ("(word %d) ", i);

	if (!(c = (char *) GetPW (pwp, i)))
	{
	    c = "(null)";
	}

	printf ("'%s'\n", c);
    }

    return (0);
}
