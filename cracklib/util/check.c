#include "config.h"
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include "crack.h"
#include <stdio.h>
#include <limits.h>
int
main(int argc, char **argv)
{
	char buf[LINE_MAX], *why;
	int i;

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
