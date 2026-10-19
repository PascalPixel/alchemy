#ifndef ALCHEMY_UNIT_GRAPHICS_CREDITS_CREDITS_H
#define ALCHEMY_UNIT_GRAPHICS_CREDITS_CREDITS_H

#include "types.h"

void Graphics_ClearCharacterBlockAndPalette(s32 alternate);
extern const u32 gDisplayScroll_SlideResources[];
void Graphics_LoadCharacterBlockAndPalette(u32 resourceId, s32 alternate);
void DisplayScroll_BuildHblankWordTable(u32 *destination);
void DisplayScroll_StepPositionEveryFourFrames(void);
s32 DisplayScroll_RunSlideshow(void);
extern const void *gDisplayScroll_LineTable[];
extern const u8 gDisplayScroll_GlyphWidths[96];
void DisplayScroll_UpdateObjects(void);
void DisplayScroll_RenderEnteringLine(void);
void DisplayScroll_InitObjectTable(void);
s32 DisplayScroll_DrawLine(const u8 *text, s32 firstTile, s32 alignment);

#endif
