#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_PSYNERGY_STONE_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_PSYNERGY_STONE_H

#include "types.h"
#include "field_scene.h"

struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSolSanctumPsynergyStoneEntrances[];
extern u32 gSolSanctumPsynergyStoneExits[];
extern struct ScenePlacement gSolSanctumPsynergyStonePlacements[];
extern struct SceneEvent gSolSanctumPsynergyStoneEvents[];

#endif
