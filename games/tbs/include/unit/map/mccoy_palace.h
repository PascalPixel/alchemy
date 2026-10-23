#ifndef ALCHEMY_UNIT_SCENE_MCCOY_PALACE_H
#define ALCHEMY_UNIT_SCENE_MCCOY_PALACE_H

#include "types.h"
#include "map_tables.h"

struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gPalace_FacingActions[];
extern s32 gPalace_PacingActions[];
extern struct SceneEntrance gMcCoyPalaceEntrances[];
extern u32 gMcCoyPalaceExits[];
extern struct ScenePlacement gMcCoyPalaceNoPlacements[];
extern struct ScenePlacement gMcCoyPalacePlacements[];
extern struct SceneEvent gMcCoyPalaceNoEvents[];
extern struct SceneEvent gMcCoyPalaceEvents[];

#endif
