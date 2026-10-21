#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_3_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_3_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *KolimaForest3_GetEntrances(void);
const struct SceneRegion *KolimaForest3_GetRegions(void);
const u32 *KolimaForest3_GetExits(void);
const struct ScenePlacement *KolimaForest3_GetPlacements(void);
const struct SceneEvent *KolimaForest3_GetEvents(void);
s32 KolimaForest3_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern struct SceneEntrance gKolimaForest3Entrances[];
extern u32 gKolimaForest3Exits[];
extern struct ScenePlacement gKolimaForest3Placements[];
extern struct SceneEvent gKolimaForest3Events[];

#endif
