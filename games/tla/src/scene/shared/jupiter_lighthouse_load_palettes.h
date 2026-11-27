/* @file */
/* The Jupiter Lighthouse rooms' palettes, which most of them keep a copy of. */

#include "palette_cycle_places.h"
#include "scene/lighthouse_palettes.h"

/* The BG palette after the cycle's, which holds its colours as they are while
 */
/* the lighthouse is unlit. */
#define UNLIT_PALETTE (CYCLE_PALETTE + 1)

/* Keeps a copy of the room's cycling palette and of UNLIT_PALETTE, then starts
 */
/* the cycle once the lighthouse is lit (FLAG_JUPITER_LIGHTHOUSE_ACTIVATED); */
/* else the cycle's places show the unlit colours. `mode` is enum */
/* LIGHTHOUSE_PALETTES. */
void scJupiterLighthouseLoadPalettes(s32 mode) {
	PaletteCyclePausesInEvents = 1;
	if(mode != LIGHTHOUSE_PALETTES_KEEP_COPIES) {
		syWordsCopyEntry(PaletteCycleColors,
						 (void *)&BG_PLTT_COLOR(CYCLE_PALETTE, 0),
						 PALETTE_BYTES);
		syWordsCopyEntry(PaletteCycleUnlitPalette,
						 (void *)&BG_PLTT_COLOR(UNLIT_PALETTE, 0),
						 PALETTE_BYTES);
	}
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) ||
	   mode == LIGHTHOUSE_PALETTES_LIT) {
		scPaletteCycleSetFrame(0);
		sySetEvent(fdPaletteCycleStep, TASK_PASS_FRAME);
	}
	else {
		syDMAXCopy(PaletteCycleUnlitColors,
				   (void *)&BG_PLTT_COLOR(CYCLE_PALETTE, CYCLE_FIRST_COLOR),
				   DMA_COPY_WORDS((CYCLE_PLACES - 1) / 2), REG_DMA3);
	}
}

#undef UNLIT_PALETTE
#undef CYCLE_PALETTE
#undef CYCLE_FIRST_COLOR
#undef CYCLE_PLACES
#undef CYCLE_STEP_FRAMES
