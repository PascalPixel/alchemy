#ifndef ALCHEMY_MODULE_SCENE_COTTAGE_ROOMS_C_H
#define ALCHEMY_MODULE_SCENE_COTTAGE_ROOMS_C_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gCottageRoomsCEntrances[];
extern u32 gCottageRoomsCExits[];
extern struct ActorPlacement gCottageRoomsCActorPlacements[];
extern struct Trigger gCottageRoomsCTriggers[];

#endif
