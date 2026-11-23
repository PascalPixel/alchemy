#ifndef ALCHEMY_CREDITS_H
#define ALCHEMY_CREDITS_H

/// @file
/// The staff credits at the end of the game: the battle backgrounds that
/// crossfade behind them, and the names scrolling up over them.

#include "types.h"

void Credits_ClearBuffer(s32 alternate);
/// The battle backgrounds shown in turn, ending with 0, which clears.
extern const u32 gCredits_Backgrounds[];
void Credits_LoadBackground(u32 resourceId, s32 alternate);
void Credits_BuildBackgroundTilemap(u32 *destination);
void Credits_ScrollBackgrounds(void);
s32 Credits_Run(void);
/// The credits' lines, one per 8 pixels of scroll, ending with NULL.
extern const void *gCredits_Lines[];
/// The credits font's widths in pixels, for characters 32 to 127.
extern const u8 gCredits_GlyphWidths[96];
void Credits_UpdateTextSprites(void);
void Credits_DrawEnteringLine(void);
void Credits_InitTextSprites(void);
s32 Credits_DrawLine(const u8 *text, s32 firstTile, s32 alignment);

#endif
