#ifndef ALCHEMY_VERSION_H
#define ALCHEMY_VERSION_H

/// @file
/// The patch levels: our groups of the measured changes from the Japanese
/// release.

#include "edition.h"

// Observed cumulative logic changes from the Japanese baseline, in the order
// the editions were released: Japanese, English, German, French, then Spanish
// and Italian together. These are our patch groups, not recovered Camelot
// version numbers. LANGUAGE chooses only what language decides (text, fonts,
// layout that fits the text); every other difference between editions takes
// a patch level, combined when no single level selects its editions: German
// alone is TLA_PATCH_2 && !TLA_PATCH_3.
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
#define TLA_PATCH_LEVEL 4
#elif defined(TLA_EDITION_FR)
#define TLA_PATCH_LEVEL 3
#elif defined(TLA_EDITION_DE)
#define TLA_PATCH_LEVEL 2
#elif defined(TLA_EDITION_EN)
#define TLA_PATCH_LEVEL 1
#else
#define TLA_PATCH_LEVEL 0
#endif

#define TLA_PATCH_1 (TLA_PATCH_LEVEL >= 1) ///< every edition but Japanese
/// German, French, Spanish and Italian.
#define TLA_PATCH_2 (TLA_PATCH_LEVEL >= 2)
#define TLA_PATCH_3 (TLA_PATCH_LEVEL >= 3) ///< French, Spanish and Italian
#define TLA_PATCH_4 (TLA_PATCH_LEVEL >= 4) ///< Spanish and Italian

#endif
