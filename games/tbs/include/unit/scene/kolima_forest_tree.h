#ifndef ALCHEMY_UNIT_MAP_KOLIMA_FOREST_TREE_H
#define ALCHEMY_UNIT_MAP_KOLIMA_FOREST_TREE_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

const struct SceneEntrance *KolimaForestTree_GetEntrances(void);
const struct SceneExitRegion *KolimaForestTree_GetRegions(void);
const u32 *KolimaForestTree_GetExits(void);
const struct ActorTemplate *KolimaForestTree_GetActorTemplates(void);
const struct MapEvent *KolimaForestTree_GetEvents(void);
s32 KolimaForestTree_Initialize(void);
extern s32 gKolimaForestTree_GaretApproachActions[];
extern s32 gKolimaForestTree_IvanApproachActions[];
extern s32 gKolimaForestTree_MiaApproachActions[];
extern s32 gKolimaForestTree_RejoinPartyActions[];
extern struct SceneEntrance gKolimaForestTreeEntrances[];
extern struct SceneExitRegion gKolimaForestTreeRegions[];
extern u32 gKolimaForestTreeExits[];
extern struct ActorTemplate gKolimaForestTreeActorTemplates[];
extern struct MapEvent gKolimaForestTreeEvents[];
extern s32 gKolimaForestTreeEffectScript[];

#endif
