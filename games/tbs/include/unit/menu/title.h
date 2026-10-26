#ifndef ALCHEMY_UNIT_MENU_TITLE_H
#define ALCHEMY_UNIT_MENU_TITLE_H

#include "types.h"
#include "map_tables.h"

u8 *Title_GetEntrances(void);
s32 Scene_GetExitRegions(void);
u8 *Title_GetExits(void);
u8 *Title_GetActorTemplates(void);
u8 *Title_GetTriggers(void);
s32 Title_Run(void);
extern struct SceneEntrance gTitleEntrances[];
extern u32 gTitleExits[];
extern struct ActorTemplate gTitleActorTemplates[];
extern struct Trigger gTitleTriggers[];
extern s16 gTitleVramBlock;

#endif
