#ifndef ALCHEMY_UNIT_SCENE_WEST_LUNPA_CAVE_H
#define ALCHEMY_UNIT_SCENE_WEST_LUNPA_CAVE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern const struct SceneEntrance gCaveEntrances[];
extern const u32 gCaveExits[];
extern const struct ScenePlacement gCaveNoPlacements[];
extern const struct ScenePlacement gCavePlacements[];
extern const struct SceneEvent gCaveNoEvents[];
extern const struct SceneEvent gCaveEvents[];

#endif
