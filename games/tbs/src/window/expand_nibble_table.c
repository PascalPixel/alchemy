/* The colour map mnGraphicsExpandVRAMTilesByColTable draws the UI's digits
   through, and the link battle timer's heading. */

#include "stubs.h"

/* The colour each pixel's colour becomes: the first sixteen draw colour 1 as
   colour 10, the second sixteen leave every colour as it is. */
const Uint16 GraphicsExpandNibbleTable[] = {
	0,
	10,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
};

#if LANGUAGE == LANGUAGE_SPANISH
const Uint8 LinkTimeLabelString[] = "TMPO.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 LinkTimeLabelString[] = "TEMPS";
#else
const Uint8 LinkTimeLabelString[] = "TIME";
#endif
