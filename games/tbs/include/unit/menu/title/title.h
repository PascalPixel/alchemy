#ifndef ALCHEMY_UNIT_MENU_TITLE_TITLE_H
#define ALCHEMY_UNIT_MENU_TITLE_TITLE_H

#include "types.h"

u8 *Title_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *Title_GetExits(void);
u8 *Title_GetPlacements(void);
u8 *Title_GetEvents(void);
s32 Title_Run(void);
void Title_LoadSprites(s32 unused);
void Title_RevealSpriteRow(void);
void Title_RevealScreen(s32 unused);
void Title_LoadBackground(void);

#endif
