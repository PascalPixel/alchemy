#ifndef ALCHEMY_UNIT_DEBUG_LEVEL_H
#define ALCHEMY_UNIT_DEBUG_LEVEL_H

#include "types.h"
#include "map_tables.h"

u8 *IncreaseItemAndLevel_GetEntrances(void);
s32 Scene_GetExitRegions(void);
u8 *IncreaseItemAndLevel_GetExits(void);
u8 *IncreaseItemAndLevel_GetActorTemplates(void);
u8 *IncreaseItemAndLevel_GetTriggers(void);
s32 IncreaseItemAndLevel_Initialize(void);
extern char gIncreaseItemAndLevel_LvLabel[4];
extern char gIncreaseItemAndLevel_ItemPrompt[12];
extern char gIncreaseItemAndLevel_ItemHelp[24];
extern char gIncreaseItemAndLevel_ItemFull[20];
extern char gIncreaseItemAndLevel_PsyPrompt[8];
extern char gIncreaseItemAndLevel_PsyHelp[12];
extern char gIncreaseItemAndLevel_GlyphsUpper[28];
extern char gIncreaseItemAndLevel_GlyphsLower[28];
extern char gIncreaseItemAndLevel_GlyphsMarks[12];
extern struct SceneEntrance gIncreaseItemAndLevel_Entrances[];
extern u32 gIncreaseItemAndLevel_Exits[];
extern struct ActorTemplate gIncreaseItemAndLevel_ActorTemplates[];
extern struct Trigger gIncreaseItemAndLevel_Triggers[];

#endif
