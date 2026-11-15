/// @file
/// The colour map Graphics_ExpandVramTilesByColorTable draws the UI's digits
/// through, and the link battle timer's heading.

#include "edition.h"
#include "graphics/graphics.h"
#include "system/system.h"

/// The colour each pixel's colour becomes: the first sixteen draw colour 1 as
/// colour 10, the second sixteen leave every colour as it is.
const u16 gGraphics_ExpandNibbleTable[] = {
    0, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    0, 1,  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

#if LANGUAGE == LANGUAGE_SPANISH
const u8 gLink_TimeLabelString[] = "TMPO.";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 gLink_TimeLabelString[] = "TEMPS";
#else
const u8 gLink_TimeLabelString[] = "TIME";
#endif
