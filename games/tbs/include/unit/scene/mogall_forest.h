#ifndef ALCHEMY_UNIT_MAP_MOGALL_FOREST_H
#define ALCHEMY_UNIT_MAP_MOGALL_FOREST_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *MogallForest_GetWarpDests(void);
const struct MapWarpRegion *MogallForest_GetRegions(void);
const u32 *MogallForest_GetWarps(void);
const struct ObjectEventTemplate *MogallForest_GetObjectEventTemplates(void);
const struct MapEvent *MogallForest_GetEvents(void);
s32 MogallForest_Initialize(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gMogallForest_RockScript[];
extern s32 gMogallForest_ObjectEventScript[];
extern s32 gMogallForest_HidingPlaceScript[];
extern struct MapWarpDest gMogallForestSign_WarpDests[];
extern struct MapWarpDest gMogallForestQuartz_WarpDests[];
extern struct MapWarpDest gMogallForestKillerApe_WarpDests[];
extern struct MapWarpDest gMogallForestSidePaths_WarpDests[];
extern u32 gMogallForest_Warps[];
extern struct ObjectEventTemplate gMogallForestSign_ObjectEventTemplates[];
extern struct ObjectEventTemplate gMogallForestQuartz_ObjectEventTemplates[];
extern struct ObjectEventTemplate gMogallForestKillerApe_ObjectEventTemplates[];
extern struct ObjectEventTemplate gMogallForestSidePaths_ObjectEventTemplates[];
extern struct MapEvent gMogallForestSign_Events[];
extern struct MapEvent gMogallForestQuartz_Events[];
extern struct MapEvent gMogallForestKillerApe_Events[];
extern struct MapEvent gMogallForestSidePaths_Events[];

#endif
