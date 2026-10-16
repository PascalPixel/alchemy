#ifndef TBS_VERSION_H
#define TBS_VERSION_H

#include "../../common/include/game/edition.h"

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

// Observed cumulative logic changes from the Japanese baseline. These are our
// patch groups, not recovered Camelot version numbers or a release chronology.
// Each conditional stays beside the behavior it changes; language choices
// use LANGUAGE independently of these groups.
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define TBS_PATCH_LEVEL 4
#elif defined(TBS_EDITION_FR)
#define TBS_PATCH_LEVEL 3
#elif defined(TBS_EDITION_DE)
#define TBS_PATCH_LEVEL 2
#elif defined(TBS_EDITION_EN)
#define TBS_PATCH_LEVEL 1
#else
#define TBS_PATCH_LEVEL 0
#endif

#define TBS_PATCH_1 (TBS_PATCH_LEVEL >= 1)
#define TBS_PATCH_2 (TBS_PATCH_LEVEL >= 2)
#define TBS_PATCH_3 (TBS_PATCH_LEVEL >= 3)
#define TBS_PATCH_4 (TBS_PATCH_LEVEL >= 4)

#endif
