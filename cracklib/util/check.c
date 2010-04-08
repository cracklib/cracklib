/*
 * This program is copyright Alec Muffett 1993, portions copyright other authors.
 * The authors disclaim all responsibility or liability with respect to it's usage
 * or its effect upon hardware or computer systems.
 */

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <locale.h>

#define IN_CRACKLIB
#include "config.h"
#include "crack.h"
#include "packer.h"

int
main(int argc, char **argv)
{
	char buf[LINE_MAX];
	const char *why;
	int i;

	setlocale(LC_ALL, "");

#ifdef ENABLE_NLS
	textdomain(PACKAGE);
#endif

	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		while (((i = strlen(buf)) > 0) && (i > 0)) {
			if (strchr("\r\n", buf[i - 1]) != NULL) {
				buf[i - 1] = '\0';
				i--;
			} else {
				break;
			}
		}
		why = FascistCheck(buf,
				   argc > 1 ?
				   argv[1] :
				   NULL);
		if ((why != NULL) && (strlen(why) > 0)) {
			printf("%s: %s\n", buf, why);
		} else {
			printf("%s: OK\n", buf);
		}
	}
	return 0;
}
