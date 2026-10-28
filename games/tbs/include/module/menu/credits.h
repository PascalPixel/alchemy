#ifndef ALCHEMY_MODULE_MENU_CREDITS_H
#define ALCHEMY_MODULE_MENU_CREDITS_H

#include "types.h"

void Credits_ClearBuffer(s32 alternate);
extern const u32 gCredits_Backgrounds[];
void Credits_LoadBackground(u32 resourceId, s32 alternate);
void Credits_BuildBackgroundTilemap(u32 *destination);
void Credits_ScrollBackgrounds(void);
s32 Credits_Run(void);
extern const void *gCredits_Lines[];
extern const u8 gCredits_GlyphWidths[96];
void Credits_UpdateTextSprites(void);
void Credits_DrawEnteringLine(void);
void Credits_InitTextSprites(void);
s32 Credits_DrawLine(const u8 *text, s32 firstTile, s32 alignment);

#endif
