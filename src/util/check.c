/*
 * This program is copyright Alec Muffett 1993, portions copyright other authors.
 * The authors disclaim all responsibility or liability with respect to it's usage
 * or its effect upon hardware or computer systems.
 */

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h>

#undef IN_CRACKLIB
#define IN_CRACKLIB

#include "config.h"
#include "crack.h"
#include "packer.h"

int
main(int argc, char **argv)
{
	char buf[LINE_MAX];
	const char *why;
	char *dbpath = NULL;
	int i;
	int result = -1;
	int echo_on = 1;
	struct termios tios;

	setlocale(LC_ALL, "");

#ifdef ENABLE_NLS
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
#endif

	if (argc == 2) {
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			fprintf(stderr, "Usage:\t%s [--no-echo]\n", argv[0]);
			fprintf(stderr, "  if --no-echo is not specified, will echo the plaintext to the terminal.\n");
			return -1;
		}

		if (!strcmp(argv[1], "--no-echo")) {
			echo_on = 0;
		} else {
			dbpath = argv[1];
		}
	}

	if (argc > 2) {
		dbpath = argv[1];
		if (!strcmp(argv[2], "--no-echo")) {
			echo_on = 0;
		}
	}

	if (!echo_on) {
		result = tcgetattr(STDIN_FILENO, &tios);
		if (!result) {
			tios.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &tios);
		}
	}

	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		while (((i = strlen(buf)) > 0) && (i > 0)) {
			if (strchr("\r\n", buf[i - 1]) != NULL) {
				buf[i - 1] = '\0';
				i--;
			} else {
				break;
			}
		}
		why = FascistCheck(buf, dbpath);
		if ((why != NULL) && (strlen(why) > 0)) {
			printf("%s: %s\n", buf, why);
		} else {
			if (!echo_on) {
				printf("It is OK\n");
			} else {
				printf("%s: OK\n", buf);
			}
		}
	}
	return 0;
}
