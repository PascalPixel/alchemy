#ifndef ALCHEMY_DEBUG_TEST_H
#define ALCHEMY_DEBUG_TEST_H

#include "types.h"
#include "field/scene_tables.h"

u8 *Scene_GetEntrances(void);
u8 *Scene_GetExits(void);
u8 *Scene_GetActorPlacements(void);
s32 Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern u32 gTest_UnusedRows[];
extern char gDebugItemPrompt[12];
extern char gDebugItemCapacityLabel[24];
extern char gDebugItemFullLabel[20];
extern struct SceneEntrance gMenuTest_Entrances[];
extern u32 gMenuTest_Exits[];
extern struct ActorPlacement gMenuTest_ActorPlacements[];
extern struct Trigger gMenuTest_Triggers[];
extern s32 gMenuTest_NextLevel;
extern s32 gMenuTest_FillLetter;

#endif
