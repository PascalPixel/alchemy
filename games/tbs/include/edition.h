#ifndef ALCHEMY_EDITION_H
#define ALCHEMY_EDITION_H

/* @file */
/* The edition's language, for C to compare (LANGUAGE == LANGUAGE_GERMAN). */

/* The build defines one TBS_EDITION_ name, as 1; none at all is Japanese. */
#if (defined(TBS_EDITION_JA) + defined(TBS_EDITION_EN) +                       \
	 defined(TBS_EDITION_DE) + defined(TBS_EDITION_ES) +                       \
	 defined(TBS_EDITION_FR) + defined(TBS_EDITION_IT)) > 1
#error Select exactly one TBS edition
#endif
#if (defined(TBS_EDITION_JA) && TBS_EDITION_JA != 1) ||                        \
	(defined(TBS_EDITION_EN) && TBS_EDITION_EN != 1) ||                        \
	(defined(TBS_EDITION_DE) && TBS_EDITION_DE != 1) ||                        \
	(defined(TBS_EDITION_ES) && TBS_EDITION_ES != 1) ||                        \
	(defined(TBS_EDITION_FR) && TBS_EDITION_FR != 1) ||                        \
	(defined(TBS_EDITION_IT) && TBS_EDITION_IT != 1)
#error Edition definitions must be 1
#endif

/* The languages LANGUAGE may be. Only text, fonts and the layout that fits */
/* them follow the language; every other difference is a patch level */
/* (version.h). */
#define LANGUAGE_JAPANESE 0
#define LANGUAGE_ENGLISH  1
#define LANGUAGE_GERMAN	  2
#define LANGUAGE_SPANISH  3
#define LANGUAGE_FRENCH	  4
#define LANGUAGE_ITALIAN  5

/* The edition's language, from the TBS_EDITION_ the build defines. */
#if defined(TBS_EDITION_EN)
#define LANGUAGE LANGUAGE_ENGLISH
#elif defined(TBS_EDITION_DE)
#define LANGUAGE LANGUAGE_GERMAN
#elif defined(TBS_EDITION_ES)
#define LANGUAGE LANGUAGE_SPANISH
#elif defined(TBS_EDITION_FR)
#define LANGUAGE LANGUAGE_FRENCH
#elif defined(TBS_EDITION_IT)
#define LANGUAGE LANGUAGE_ITALIAN
#else
#define LANGUAGE LANGUAGE_JAPANESE
#endif

#endif
