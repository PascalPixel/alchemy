#ifndef ALCHEMY_UNIT_MAP_TIMBER_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_TIMBER_ROOMS_A_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gTimberRoomsAEntrances[];
extern struct SceneExitRegion gTimberRoomsARegions[];
extern u32 gTimberRoomsAExits[];
extern struct ActorPlacement gTimberRoomsAActorPlacements[];
extern struct ActorPlacement gTimberRoomsAActorPlacementsAfterAudience[];
extern struct Trigger gTimberRoomsATriggers[];
extern struct Trigger gTimberRoomsATriggersAfterAudience[];

#endif
