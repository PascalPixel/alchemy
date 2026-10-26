#ifndef ALCHEMY_UNIT_DEBUG_TEST_H
#define ALCHEMY_UNIT_DEBUG_TEST_H

#include "types.h"
#include "map_tables.h"

u8 *MenuTest_GetEntrances(void);
s32 Scene_GetExitRegions(void);
u8 *MenuTest_GetExits(void);
u8 *MenuTest_GetActorTemplates(void);
s32 MenuTest_GetEvents(void);
s32 MenuTest_Initialize(void);
extern u32 gDebugItemPromptHead[];
extern char gDebugItemPrompt[12];
extern char gDebugItemCapacityLabel[24];
extern char gDebugItemFullLabel[20];
extern struct SceneEntrance gMenuTest_Entrances[];
extern u32 gMenuTest_Exits[];
extern struct ActorTemplate gMenuTest_ActorTemplates[];
extern struct MapEvent gMenuTest_Events[];
extern s32 gMenuTest_SlotOffsets;
extern s32 gMenuTest_FillLetter;

#endif
