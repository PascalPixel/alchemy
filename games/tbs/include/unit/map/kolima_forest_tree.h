#ifndef ALCHEMY_UNIT_MAP_KOLIMA_FOREST_TREE_H
#define ALCHEMY_UNIT_MAP_KOLIMA_FOREST_TREE_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"

const struct MapWarpDest *KolimaForestTree_GetWarpDests(void);
const struct MapWarpRegion *KolimaForestTree_GetRegions(void);
const u32 *KolimaForestTree_GetWarps(void);
const struct ObjectEventTemplate *
KolimaForestTree_GetObjectEventTemplates(void);
const struct MapEvent *KolimaForestTree_GetEvents(void);
s32 KolimaForestTree_Initialize(void);
extern s32 gKolimaForestTree_GaretApproachActions[];
extern s32 gKolimaForestTree_IvanApproachActions[];
extern s32 gKolimaForestTree_MiaApproachActions[];
extern s32 gKolimaForestTree_RejoinPartyActions[];
extern struct MapWarpDest gKolimaForestTreeWarpDests[];
extern struct MapWarpRegion gKolimaForestTreeRegions[];
extern u32 gKolimaForestTreeWarps[];
extern struct ObjectEventTemplate gKolimaForestTreeObjectEventTemplates[];
extern struct MapEvent gKolimaForestTreeEvents[];
extern s32 gKolimaForestTreeEffectScript[];

#endif
