#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_2_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_2_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *KolimaForest2_GetEntrances(void);
const struct SceneRegion *KolimaForest2_GetRegions(void);
const u32 *KolimaForest2_GetExits(void);
const struct ScenePlacement *KolimaForest2_GetPlacements(void);
const struct SceneEvent *KolimaForest2_GetEvents(void);
s32 KolimaForest2_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern struct SceneEntrance gKolimaForest2_Entrances[];
extern u32 gKolimaForest2_Exits[];
extern struct ScenePlacement gKolimaForest2_Placements[];
extern struct SceneEvent gKolimaForest2_Events[];

#endif
