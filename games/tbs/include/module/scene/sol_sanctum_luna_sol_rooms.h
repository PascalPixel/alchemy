#ifndef ALCHEMY_MODULE_SCENE_SOL_SANCTUM_LUNA_SOL_ROOMS_H
#define ALCHEMY_MODULE_SCENE_SOL_SANCTUM_LUNA_SOL_ROOMS_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSolSanctumLunaSolRoomsEntrances[];
extern u32 gSolSanctumLunaSolRoomsExits[];
extern struct ActorPlacement gSolSanctumLunaSolRoomsActorPlacements[];
extern struct Trigger gSolSanctumLunaSolRoomsTriggers[];

#endif
