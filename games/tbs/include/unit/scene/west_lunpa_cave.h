#ifndef ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H
#define ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern const struct SceneEntrance gCaveEntrances[];
extern const u32 gCaveExits[];
extern const struct ActorPlacement gCaveNoActorPlacements[];
extern const struct ActorPlacement gCaveActorPlacements[];
extern const struct Trigger gCaveNoTriggers[];
extern const struct Trigger gCaveTriggers[];

#endif
