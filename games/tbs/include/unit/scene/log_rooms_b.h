#ifndef ALCHEMY_UNIT_MAP_LOG_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_LOG_ROOMS_B_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gLogRoomsB_Entrances[];
extern u32 gLogRoomsB_Exits[];
extern struct ActorPlacement gLogRoomsB_ActorPlacements[];
extern struct Trigger gLogRoomsB_Triggers[];

#endif
