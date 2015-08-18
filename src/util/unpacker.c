/*
 * This program is copyright Alec Muffett 1993, portions copyright other authors.
 * The authors disclaim all responsibility or liability with respect to it's usage
 * or its effect upon hardware or computer systems.
 */

#include <stdio.h>

#define IN_CRACKLIB
#include "config.h"
#include "crack.h"
#include "packer.h"

int
main(argc, argv)
    int argc;
    char *argv[];
{
    uint32_t i;
    PWDICT *pwp;

    if (argc <= 1)
    {
	fprintf(stderr, "Usage:\t%s dbname\n", argv[0]);
	return (-1);
    }

    if (!(pwp = PWOpen (argv[1], "r")))
    {
	perror ("PWOpen");
	return (-1);
    }

    for (i=0; i < PW_WORDS(pwp); i++)
    {
    	register char *c;

	if (!(c = (char *) GetPW (pwp, i)))
	{
	    fprintf(stderr, "error: GetPW %d failed\n", i);
	    continue;
	}

	printf ("%s\n", c);
    }

    return (0);
}
