#ifndef GUARD_EDITION_H
#define GUARD_EDITION_H

#if (defined(TLA_EDITION_JA) + defined(TLA_EDITION_EN) +                       \
     defined(TLA_EDITION_DE) + defined(TLA_EDITION_ES) +                       \
     defined(TLA_EDITION_FR) + defined(TLA_EDITION_IT)) > 1
#error multiple game editions selected
#endif

// Compile-time language selectors.
#define LANGUAGE_JAPANESE 0
#define LANGUAGE_ENGLISH 1
#define LANGUAGE_GERMAN 2
#define LANGUAGE_SPANISH 3
#define LANGUAGE_FRENCH 4
#define LANGUAGE_ITALIAN 5

#if defined(TLA_EDITION_EN)
#define LANGUAGE LANGUAGE_ENGLISH
#elif defined(TLA_EDITION_DE)
#define LANGUAGE LANGUAGE_GERMAN
#elif defined(TLA_EDITION_ES)
#define LANGUAGE LANGUAGE_SPANISH
#elif defined(TLA_EDITION_FR)
#define LANGUAGE LANGUAGE_FRENCH
#elif defined(TLA_EDITION_IT)
#define LANGUAGE LANGUAGE_ITALIAN
#else
#define LANGUAGE LANGUAGE_JAPANESE
#endif

#endif
