#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_POND_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_POND_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gGomaCavePondEntrances[];
extern u32 gGomaCavePondExits[];
extern struct ScenePlacement gGomaCavePondPlacements[];
extern struct SceneEvent gGomaCavePondEvents[];

#endif
