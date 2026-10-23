#ifndef ALCHEMY_UNIT_MAP_BABI_LIGHTHOUSE_H
#define ALCHEMY_UNIT_MAP_BABI_LIGHTHOUSE_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *BabiLighthouse_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 BabiLighthouse_SetupMap(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gBabiLighthouse_TrembleScript[];
extern struct MapWarpDest gBabiLighthouse_EntryWarpDests[];
extern struct MapWarpDest gBabiLighthouse_LuffWarpDests[];
extern struct MapWarpDest gBabiLighthouse_StoneDoorWarpDests[];
extern struct MapWarpDest gBabiLighthouse_RoadWarpDests[];
extern struct MapWarpRegion gBabiLighthouse_EntryRegions[];
extern u32 gBabiLighthouse_Warps[];
extern struct ObjectEventTemplate gBabiLighthouse_NoObjectEventTemplates[];
extern struct ObjectEventTemplate gBabiLighthouse_EntryObjectEventTemplates[];
extern struct ObjectEventTemplate gBabiLighthouse_LuffObjectEventTemplates[];
extern struct ObjectEventTemplate
    gBabiLighthouse_StoneDoorObjectEventTemplates[];
extern struct ObjectEventTemplate gBabiLighthouse_RoadObjectEventTemplates[];
extern struct MapEvent gBabiLighthouse_EntryEvents[];
extern struct MapEvent gBabiLighthouse_LuffEvents[];
extern struct MapEvent gBabiLighthouse_StoneDoorEvents[];
extern struct MapEvent gBabiLighthouse_RoadEvents[];

#endif
