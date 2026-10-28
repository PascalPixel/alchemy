#ifndef ALCHEMY_MODULE_DEBUG_TEST_SCENES_H
#define ALCHEMY_MODULE_DEBUG_TEST_SCENES_H

#include "types.h"
#include "scene_tables.h"

u8 *Test_GetEntrances(void);
s32 Scene_GetExitRegions(void);
u8 *Test_GetExits(void);
u8 *Test_GetActorPlacements(void);
u8 *Test_GetTriggers(void);
s32 Test_Initialize(void);
extern u32 gTest_EntrancesHead[];
extern struct SceneEntrance gTest_Entrances[];
extern u32 gTest_Exits[];
extern struct ActorPlacement gTest_ActorPlacements[];
extern struct Trigger gTest_Triggers[];

#endif
