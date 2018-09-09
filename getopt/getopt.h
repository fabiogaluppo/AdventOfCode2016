#ifndef GETOPT_H
#define GETOPT_H

/*
getopt
declaration: http://svnweb.freebsd.org/base/stable/9/include/unistd.h?view=markup
definition: http://svnweb.freebsd.org/base/stable/9/lib/libc/stdlib/getopt.c?view=markup
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int opterr = 1, optind = 1, optopt, optreset;
char *optarg;

#define BADCH   (int)'?'
#define BADARG  (int)':'
#define EMSG    ""

/*
* getopt --
*      Parse argc/argv argument vector.
*/
int getopt(int nargc, char * const nargv[], const char *ostr)
{
	static char *place = EMSG;              /* option letter processing */
	char *oli;                              /* option letter list index */

	if (optreset || *place == 0) {          /* update scanning pointer */
		optreset = 0;
		place = nargv[optind];
		if (optind >= nargc || *place++ != '-') {
			/* Argument is absent or is not an option */
			place = EMSG;
			return (-1);
		}
		optopt = *place++;
		if (optopt == '-' && *place == 0) {
			++optind;
			place = EMSG;
			return (-1);
		}
		if (optopt == 0) {
			/* Solitary '-', treat as a '-' option
			if the program (eg su) is looking for it. */
			place = EMSG;
			if (strchr(ostr, '-') == NULL)
				return (-1);
			optopt = '-';
		}
	}
	else
		optopt = *place++;

	/* See if option letter is one the caller wanted... */
	if (optopt == ':' || (oli = const_cast<char*>(strchr(ostr, optopt))) == NULL) {
		if (*place == 0)
			++optind;
		if (opterr && *ostr != ':')
			(void) fprintf(stderr,
				"%s: illegal option -- %c\n", "getopt",
				optopt);
		return (BADCH);
	}

	/* Does this option need an argument? */
	if (oli[1] != ':') {
		/* don't need argument */
		optarg = NULL;
		if (*place == 0)
			++optind;
	}
	else {
		/* Option-argument is either the rest of this argument or the
		entire next argument. */
		if (*place)
			optarg = place;
		else if (nargc > ++optind)
			optarg = nargv[optind];
		else {
			/* option-argument absent */
			place = EMSG;
			if (*ostr == ':')
				return (BADARG);
			if (opterr)
				(void)fprintf(stderr,
					"%s: option requires an argument -- %c\n",
					"getopt", optopt);
			return (BADCH);
		}
		place = EMSG;
		++optind;
	}
	return (optopt); /* return option letter */
}

#endif /* GETOPT_H */ 