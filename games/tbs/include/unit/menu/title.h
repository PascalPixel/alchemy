#ifndef ALCHEMY_UNIT_MENU_TITLE_H
#define ALCHEMY_UNIT_MENU_TITLE_H

#include "types.h"
#include "map_tables.h"

u8 *Title_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *Title_GetExits(void);
u8 *Title_GetPlacements(void);
u8 *Title_GetEvents(void);
s32 Title_Run(void);
extern struct SceneEntrance gTitleEntrances[];
extern u32 gTitleExits[];
extern struct ScenePlacement gTitlePlacements[];
extern struct SceneEvent gTitleEvents[];
extern s16 gTitleVramBlock;

#endif
