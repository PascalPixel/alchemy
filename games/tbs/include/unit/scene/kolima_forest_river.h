#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_RIVER_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_RIVER_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *KolimaForestRiver_GetEntrances(void);
const struct SceneRegion *KolimaForestRiver_GetRegions(void);
const u32 *KolimaForestRiver_GetExits(void);
const struct ScenePlacement *KolimaForestRiver_GetPlacements(void);
const struct SceneEvent *KolimaForestRiver_GetEvents(void);
s32 KolimaForestRiver_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern struct SceneEntrance gKolimaForestRiverEntrances[];
extern u32 gKolimaForestRiverExits[];
extern struct ScenePlacement gKolimaForestRiverPlacements[];
extern struct SceneEvent gKolimaForestRiverEvents[];

#endif
