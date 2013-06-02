/*
 * This program is copyright Alec Muffett 1993, portions copyright other authors.
 * The authors disclaim all responsibility or liability with respect to it's usage
 * or its effect upon hardware or computer systems.
 */

#include <stdio.h>
#include <string.h>

#define IN_CRACKLIB
#include "config.h"
#include "crack.h"
#include "packer.h"

int
main(argc, argv)
    int argc;
    char *argv[];
{
    unsigned long readed;
    unsigned long wrote;
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

    if ( argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) )
    {
	fprintf(stderr, "Usage:\t%s dbname\n", argv[0]);
        fprintf(stderr, "  if dbname is not specified, will use compiled in default of (%s).\n", DEFAULT_CRACKLIB_DICT);
	return (-1);
    }

    if (!(pwp = PWOpen(file, "w")))
    {
	perror(file);
	return (-1);
    }

    wrote = 0;

    for (readed = 0; fgets(buffer, STRINGSIZE, stdin); /* nothing */)
    {
    	readed++;

	buffer[MAXWORDLEN - 1] = '\0';

	Chop(buffer);

	if (!buffer[0])
	{
	    fprintf(stderr, "skipping line: %lu\n", readed);
	    continue;
	}

	if (PutPW(pwp, buffer))
	{
	    fprintf(stderr, "error: PutPW '%s' line %luy\n", buffer, readed);
	}

	wrote++;
    }

    PWClose(pwp);

    printf("%lu %lu\n", readed, wrote);

    return (0);
}
