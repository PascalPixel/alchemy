#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_CLIFF_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_CLIFF_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *KolimaForestCliff_GetEntrances(void);
const struct SceneRegion *KolimaForestCliff_GetRegions(void);
const u32 *KolimaForestCliff_GetExits(void);
const struct ScenePlacement *KolimaForestCliff_GetPlacements(void);
const struct SceneEvent *KolimaForestCliff_GetEvents(void);
s32 KolimaForestCliff_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern struct SceneEntrance gKolimaForestCliffEntrances[];
extern u32 gKolimaForestCliffExits[];
extern struct ScenePlacement gKolimaForestCliffPlacements[];
extern struct SceneEvent gKolimaForestCliffEvents[];

#endif
