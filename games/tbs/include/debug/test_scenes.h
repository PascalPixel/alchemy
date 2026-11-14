#ifndef ALCHEMY_DEBUG_TEST_SCENES_H
#define ALCHEMY_DEBUG_TEST_SCENES_H

#include "types.h"
#include "field/scene_tables.h"

u8 *Scene_GetEntrances(void);
u8 *Scene_GetExits(void);
u8 *Scene_GetActorPlacements(void);
u8 *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gTest_Entrances[];
extern u32 gTest_Exits[];
extern struct ActorPlacement gTest_ActorPlacements[];
extern struct Trigger gTest_Triggers[];

#endif
