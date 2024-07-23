/*
 * This program is copyright Alec Muffett 1993, portions copyright other authors.
 * The authors disclaim all responsibility or liability with respect to it's usage
 * or its effect upon hardware or computer systems.
 */

#undef IN_CRACKLIB
#define IN_CRACKLIB

#include "config.h"
#include "crack.h"
#include "packer.h"

int
main(int argc, char *argv[])
{
    PWDICT *pwp;
    char buffer[STRINGSIZE];
    char *file;

    if (argc <= 1)
    {
	file = DEFAULT_CRACKLIB_DICT;
    }
    else
    {
        file = argv[1];
    }

    if (!(pwp = PWOpen (file, "r")))
    {
	perror ("PWOpen");
	return (-1);
    }

    printf("enter dictionary words, one per line...\n");

    while (fgets (buffer, STRINGSIZE, stdin))
    {
    	uint32_t i;

	Chop(buffer);
	i = FindPW(pwp, buffer);
	printf ("%s: %u%s\n", buffer, i, (i==PW_WORDS(pwp) ? " **** NOT FOUND ****" : ""));
    }

    return (0);
}
