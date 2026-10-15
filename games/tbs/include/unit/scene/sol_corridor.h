#ifndef ALCHEMY_UNIT_SCENE_SOL_CORRIDOR_H
#define ALCHEMY_UNIT_SCENE_SOL_CORRIDOR_H

#include "types.h"
#include "field_scene.h"

struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
extern struct SceneEntrance gSolCorridorEntrances[];
extern u32 gSolCorridorExits[];
extern struct ScenePlacement gSolCorridorPlacements[];
extern struct SceneEvent gSolCorridorEvents[];

#endif
