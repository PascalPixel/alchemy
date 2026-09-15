#ifndef TBS_VERSION_H
#define TBS_VERSION_H

/* The English production build is the default; edition builds select one. */
#if (defined(TBS_EDITION_JA) + defined(TBS_EDITION_EN) + defined(TBS_EDITION_DE) + defined(TBS_EDITION_ES) + defined(TBS_EDITION_FR) + defined(TBS_EDITION_IT)) > 1
#error Select exactly one TBS edition
#endif
#if !defined(TBS_EDITION_JA) && !defined(TBS_EDITION_EN) && !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
#define TBS_EDITION_EN 1
#endif
#if (defined(TBS_EDITION_JA) && TBS_EDITION_JA != 1) || (defined(TBS_EDITION_EN) && TBS_EDITION_EN != 1) || (defined(TBS_EDITION_DE) && TBS_EDITION_DE != 1) || (defined(TBS_EDITION_ES) && TBS_EDITION_ES != 1) || (defined(TBS_EDITION_FR) && TBS_EDITION_FR != 1) || (defined(TBS_EDITION_IT) && TBS_EDITION_IT != 1)
#error Edition definitions must be 1
#endif

#endif
