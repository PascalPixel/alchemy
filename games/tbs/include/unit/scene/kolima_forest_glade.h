#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_GLADE_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_GLADE_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *KolimaForestGlade_GetEntrances(void);
const struct SceneRegion *KolimaForestGlade_GetRegions(void);
const u32 *KolimaForestGlade_GetExits(void);
const struct ScenePlacement *KolimaForestGlade_GetPlacements(void);
const struct SceneEvent *KolimaForestGlade_GetEvents(void);
s32 KolimaForestGlade_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern struct SceneEntrance gKolimaForestGlade_Entrances[];
extern u32 gKolimaForestGlade_Exits[];
extern struct ScenePlacement gKolimaForestGlade_Placements[];
extern struct SceneEvent gKolimaForestGlade_Events[];

#endif
