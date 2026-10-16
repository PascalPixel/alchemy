// Build switches
// Build switches, as pret's config.h: BUGFIX opts in to fixes of the
// original game's bugs, and UBFIX gives undefined behaviour its defined
// form. The matching build defines neither; the modern build defines UBFIX.
#ifndef ALCHEMY_CONFIG_H
#define ALCHEMY_CONFIG_H

// Uncomment to fix some identified minor bugs
// #define BUGFIX

// Various undefined behavior bugs may or may not prevent compilation with
// newer compilers. So always fix them when using a modern compiler.
#if MODERN || defined(BUGFIX)
#ifndef UBFIX
#define UBFIX
#endif
#endif

#endif
