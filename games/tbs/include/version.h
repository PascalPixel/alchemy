#ifndef ALCHEMY_VERSION_H
#define ALCHEMY_VERSION_H

/// @file
/// The patch levels: our groups of the measured changes from the Japanese
/// release.

#include "edition.h"

/// How far an edition's code has moved from the Japanese release's, each level
/// keeping the changes of those below it. The levels group the changes the
/// editions were measured to share; they are not Camelot's version numbers or
/// the order of release. A test of one stands beside the code it changes.
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

#define TBS_PATCH_1 (TBS_PATCH_LEVEL >= 1) ///< all but Japanese
#define TBS_PATCH_2 (TBS_PATCH_LEVEL >= 2) ///< German, French, Spanish, Italian
#define TBS_PATCH_3 (TBS_PATCH_LEVEL >= 3) ///< French, Spanish and Italian
#define TBS_PATCH_4 (TBS_PATCH_LEVEL >= 4) ///< Spanish and Italian

#endif
