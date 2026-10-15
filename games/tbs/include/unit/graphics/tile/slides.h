#ifndef ALCHEMY_UNIT_GRAPHICS_TILE_SLIDES_H
#define ALCHEMY_UNIT_GRAPHICS_TILE_SLIDES_H

#include "types.h"

void               Graphics_ClearCharacterBlockAndPalette(s32 alt);
extern const u32   gDisplayScroll_SlideResources[];
void               Graphics_LoadCharacterBlockAndPalette(u32 resource, s32 alternate);
void               DisplayScroll_BuildHblankWordTable(u32 *arg0);
void               DisplayScroll_StepPositionEveryFourFrames(void);
s32                DisplayScroll_RunSlideshow(void);
extern const void *gDisplayScroll_LineTable[];
extern const u8    gDisplayScroll_GlyphWidths[96];
void               DisplayScroll_UpdateObjects(void);
void               DisplayScroll_RenderEnteringLine(void);
void               DisplayScroll_InitObjectTable(void);
s32                DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

#endif
