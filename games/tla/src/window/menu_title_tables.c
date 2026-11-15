/// @file
/// The menu titles' tables: the colour map of the doubled digit tiles, and
/// the link timer's label.

#include "types.h"
#include "edition.h"

/// Each 4bpp colour as Graphics_ExpandVramTilesByColorTable maps it: the
/// first sixteen with colour 1 as colour 10, then the sixteen unchanged.
// clang-format off
u16 gGraphics_ExpandNibbleTable[32] = {
    0, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
// clang-format on

#if LANGUAGE == LANGUAGE_SPANISH
u8 gLink_TimeLabelString[] = "TMPO.";
#elif LANGUAGE == LANGUAGE_FRENCH
u8 gLink_TimeLabelString[] = "TEMPS";
#elif LANGUAGE == LANGUAGE_ITALIAN
u8 gLink_TimeLabelString[] = "TEMPO";
#else
u8 gLink_TimeLabelString[] = "TIME";
#endif
