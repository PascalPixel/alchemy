#ifndef ALCHEMY_CREDITS_H
#define ALCHEMY_CREDITS_H

/* The staff credits at the end of the game: the battle backgrounds that
   crossfade behind them, and the names scrolling up over them. */

#include "types.h"

/* The credits module's functions in the order of its stub table
   (src/menu/credits_stubs.s). */
Sint32 mnCreditsRun(void);

void mnCreditsClearBuffer(Sint32 alternate);
/* The battle backgrounds shown in turn, ending with 0, which clears. */
extern const Uint32 CreditsBackgrounds[];
void mnCreditsLoadBackground(Uint32 resourceId, Sint32 alternate);
void mnCreditsBuildBackgroundTilemap(Uint32 *dest);
void mnCreditsScrollBackgrounds(void);
/* The credits' lines, one per 8 pixels of scroll, ending with NULL. */
extern const void *CreditsLines[];
/* The credits font's widths in pixels, for characters 32 to 127. */
extern const Uint8 CreditsGlyphWidths[96];
void			   mnCreditsUpdateTextSprites(void);
void			   mnCreditsDrawEnteringLine(void);
void			   mnCreditsInitTextSprites(void);
Sint32 mnCreditsDrawLine(const Uint8 *text, Sint32 firstTile, Sint32 alignment);

#endif
