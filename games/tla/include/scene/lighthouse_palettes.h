#ifndef ALCHEMY_LIGHTHOUSE_PALETTES_H
#define ALCHEMY_LIGHTHOUSE_PALETTES_H

/* @file */
/* How the Jupiter Lighthouse rooms, and one Mars Lighthouse room that keeps a
 */
/* copy, load their cycling palette */
/* (shared/jupiter_lighthouse_load_palettes.h). */

#include "types.h"

/* How scJupiterLighthouseLoadPalettes treats a room. */
typedef enum LIGHTHOUSE_PALETTES {
	LIGHTHOUSE_PALETTES_BY_FLAG,	/* lit once the lighthouse is */
	LIGHTHOUSE_PALETTES_LIT,		/* lit whatever the flag */
	LIGHTHOUSE_PALETTES_KEEP_COPIES /* by the flag, keeping the copies it has */
} LIGHTHOUSE_PALETTES;

void scJupiterLighthouseLoadPalettes(s32 mode);

#endif
