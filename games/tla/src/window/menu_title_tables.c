/* The colour map of the page number tiles, built twice from the UI's digits,
   and the link battle timer's label. */

#include "types.h"
#include "edition.h"

/* clang-format off */
/* Each 4bpp colour as mnGraphicsExpandVRAMTilesByColTable maps it: the */
/* first sixteen with colour 1 as colour 10, then the sixteen unchanged. */
Uint16 Graphics_ExpandNibbleTable[32] = {
	0, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
/* clang-format on */

/* The label of the link battle's countdown window
   (mnLinkCreateCountdownLabelWindow). */
#if LANGUAGE == LANGUAGE_SPANISH
Uint8 Link_TimeLabelString[] = "TMPO.";
#elif LANGUAGE == LANGUAGE_FRENCH
Uint8 Link_TimeLabelString[] = "TEMPS";
#elif LANGUAGE == LANGUAGE_ITALIAN
Uint8 Link_TimeLabelString[] = "TEMPO";
#else
Uint8 Link_TimeLabelString[] = "TIME";
#endif
