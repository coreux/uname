/*
 * Copyright 2007-2012 Mo McRoberts.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
 
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_SYS_UTSNAME_H
# include <sys/utsname.h>
#endif
#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif
#ifdef HAVE_NL_TYPES_H
# include <nl_types.h>
#endif

#ifndef EXIT_FAILURE
# define EXIT_FAILURE                   1
#endif

#define UNAME_SYSTEM                    (1<<0)
#define UNAME_NODENAME                  (1<<1)
#define UNAME_RELEASE                   (1<<2)
#define UNAME_VERSION                   (1<<3)
#define UNAME_MACHINE                   (1<<4)

#define UNAME_DEFAULT                   UNAME_SYSTEM

#define UNAME_ALL \
	(UNAME_SYSTEM|UNAME_NODENAME|UNAME_RELEASE|UNAME_VERSION|UNAME_MACHINE)

#define UNAME_MSG_USAGE                 1

static const char *short_program_name = "uname";

#ifdef ENABLE_NLS
static nl_catd msgcat;
#endif

#undef _
#ifdef ENABLE_NLS
# define _(i,x)                         catgets(msgcat, NL_SETD, i, x)
#else
# define _(i,x)                         (x)
#endif

static void
usage(void)
{
	fprintf(stderr, _(UNAME_MSG_USAGE, "Usage: %s [-snrvma]"), short_program_name);
	fputc('\n', stderr);
}

int
main(int argc, char **argv)
{
	unsigned int flags;
	int ch, o;
	const char *t;
	struct utsname utsname;

#ifdef ENABLE_NLS
	setlocale(LC_ALL, "");
	msgcat = catopen("UXuname", NL_CAT_LOCALE);
#endif
	if(argv[0])
	{
		if(NULL != (t = strrchr(argv[0], '/')))
		{
			short_program_name = t + 1;
		}
		else
		{
			short_program_name = argv[0];
		}
		argv[0] = (char *) short_program_name;
	}
	flags = 0;
	while((ch = getopt(argc, argv, "snrvma")) != -1)
	{
		switch(ch)
		{
		case 's':
			flags |= UNAME_SYSTEM;
			break;
		case 'n':
			flags |= UNAME_NODENAME;
			break;
		case 'r':
			flags |= UNAME_RELEASE;
			break;
		case 'v':
			flags |= UNAME_VERSION;
			break;
		case 'm':
			flags |= UNAME_MACHINE;
			break;
		case 'a':
			flags |= UNAME_ALL;
			break;
		default:
			usage();
			exit(EXIT_FAILURE);
		}
	}
	if(optind != argc)
	{
		usage();
		exit(EXIT_FAILURE);
	}
	if(0 == flags)
	{
		flags = UNAME_DEFAULT;
	}
	if(0 != uname(&utsname))
	{
		perror(short_program_name);
		exit(EXIT_FAILURE);
	}
	o = 0;
	if(flags & UNAME_SYSTEM)
	{
		fputs(utsname.sysname, stdout);
		o++;
	}
	if(flags & UNAME_NODENAME)
	{
		if(o)
		{
			putchar(' ');
		}
		fputs(utsname.nodename, stdout);
		o++;
	}
	if(flags & UNAME_RELEASE)
	{
		if(o)
		{
			putchar(' ');
		}
		fputs(utsname.release, stdout);
		o++;
	}
	if(flags & UNAME_VERSION)
	{
		if(o)
		{
			putchar(' ');
		}
		fputs(utsname.version, stdout);
		o++;
	}
	if(flags & UNAME_MACHINE)
	{
		if(o)
		{
			putchar(' ');
		}
		fputs(utsname.machine, stdout);
	}
	putchar('\n');
#ifdef ENABLE_NLS
	if(msgcat)
	{
		catclose(msgcat);
	}
#endif
	return 0;
}
