#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_CLEARING_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_CLEARING_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *KolimaForestClearing_GetEntrances(void);
const struct SceneRegion *KolimaForestClearing_GetRegions(void);
const u32 *KolimaForestClearing_GetExits(void);
const struct ScenePlacement *KolimaForestClearing_GetPlacements(void);
const struct SceneEvent *KolimaForestClearing_GetEvents(void);
s32 KolimaForestClearing_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern struct SceneEntrance gKolimaForestClearing_Entrances[];
extern u32 gKolimaForestClearing_Exits[];
extern struct ScenePlacement gKolimaForestClearing_Placements[];
extern struct SceneEvent gKolimaForestClearing_Events[];

#endif
