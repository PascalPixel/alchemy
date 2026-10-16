#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_TREE_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_TREE_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

const struct SceneEntrance *KolimaForestTree_GetEntrances(void);
const struct SceneRegion *KolimaForestTree_GetRegions(void);
const u32 *KolimaForestTree_GetExits(void);
const struct ScenePlacement *KolimaForestTree_GetPlacements(void);
const struct SceneEvent *KolimaForestTree_GetEvents(void);
s32 KolimaForestTree_Initialize(void);
extern s32 gKolimaForestTree_GaretApproachActions[];
extern s32 gKolimaForestTree_IvanApproachActions[];
extern s32 gKolimaForestTree_MiaApproachActions[];
extern s32 gKolimaForestTree_RejoinPartyActions[];
extern struct SceneEntrance gKolimaForestTreeEntrances[];
extern struct SceneRegion gKolimaForestTreeRegions[];
extern u32 gKolimaForestTreeExits[];
extern struct ScenePlacement gKolimaForestTreePlacements[];
extern struct SceneEvent gKolimaForestTreeEvents[];
extern s32 gKolimaForestTreeEffectScript[];

#endif
