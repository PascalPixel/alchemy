#ifndef ALCHEMY_UNIT_MAP_BILIBIN_CAVE_H
#define ALCHEMY_UNIT_MAP_BILIBIN_CAVE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *BilibinCave_GetWarpDests(void);
const struct MapWarpRegion *BilibinCave_GetRegions(void);
const u32 *BilibinCave_GetWarps(void);
const struct ObjectEventTemplate *BilibinCave_GetObjectEventTemplates(void);
const struct MapEvent *BilibinCave_GetEvents(void);
s32 BilibinCave_Initialize(void);
extern s32 gBilibinCave_ClearSpriteScript[];
extern struct MapWarpDest gBilibinCaveWarpDestsOther[];
extern struct MapWarpDest gBilibinCaveEntranceWarpDests[];
extern struct MapWarpDest gBilibinCaveRapierWarpDests[];
extern struct MapWarpDest gBilibinCaveIceWarpDests[];
extern u32 gBilibinCaveWarps[];
extern struct ObjectEventTemplate gBilibinCaveObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gBilibinCaveEntranceObjectEventTemplates[];
extern struct ObjectEventTemplate gBilibinCaveRapierObjectEventTemplates[];
extern struct ObjectEventTemplate gBilibinCaveIceObjectEventTemplates[];
extern struct MapEvent gBilibinCaveEventsOther[];
extern struct MapEvent gBilibinCaveEntranceEvents[];
extern struct MapEvent gBilibinCaveRapierEvents[];
extern struct MapEvent gBilibinCaveIceEvents[];
extern s32 gBilibinCave_PushSteps[];

#endif
