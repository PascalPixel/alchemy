#ifndef ALCHEMY_CREDITS_H
#define ALCHEMY_CREDITS_H

/* @file */
/* The staff credits at the end of the game: the battle backgrounds that */
/* crossfade behind them, and the names scrolling up over them. */

#include "types.h"

/* The credits module's functions in the order of its stub table */
/* (src/menu/credits_stubs.s). */
s32 mnCreditsRun(void);

void mnCreditsClearBuffer(s32 alternate);
/* The battle backgrounds shown in turn, ending with 0, which clears. */
extern const u32 Credits_Backgrounds[];
void mnCreditsLoadBackground(u32 resourceId, s32 alternate);
void mnCreditsBuildBackgroundTilemap(u32 *destination);
void mnCreditsScrollBackgrounds(void);
/* The credits' lines, one per 8 pixels of scroll, ending with NULL. */
extern const void *Credits_Lines[];
/* The credits font's widths in pixels, for characters 32 to 127. */
extern const u8 Credits_GlyphWidths[96];
void mnCreditsUpdateTextSprs(void);
void mnCreditsDrawEnteringLine(void);
void mnCreditsInitTextSprs(void);
s32 mnCreditsDrawLine(const u8 *text, s32 firstTile, s32 alignment);

#endif
