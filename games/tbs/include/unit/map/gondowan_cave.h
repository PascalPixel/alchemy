#ifndef ALCHEMY_UNIT_MAP_GONDOWAN_CAVE_H
#define ALCHEMY_UNIT_MAP_GONDOWAN_CAVE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedObjectEvent_DirectionSteps[];
extern u32 gStagedObjectEvent_FootprintKinds[];
extern s32 gStagedObjectEvent_FootprintBounds[];
extern const s32 *gEffectScripts[];
extern struct MapWarpDest gGondowanCaveWarpDestsOther[];
extern struct MapWarpDest gGondowanPassageWarpDests[];
extern struct MapWarpDest gGondowanCaveRiverWarpDests[];
extern struct MapWarpDest gGondowanCaveWarpDests[];
extern u32 gGondowanCaveWarps[];
extern struct ObjectEventTemplate gGondowanCaveObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gGondowanPassageObjectEventTemplates[];
extern struct ObjectEventTemplate
    gGondowanPassageObjectEventTemplatesWithIodem[];
extern struct ObjectEventTemplate gGondowanCaveRiverObjectEventTemplates[];
extern struct ObjectEventTemplate gGondowanCaveObjectEventTemplates[];
extern struct MapEvent gGondowanCaveEventsOther[];
extern struct MapEvent gGondowanPassageEvents[];
extern struct MapEvent gGondowanPassageEventsWithIodem[];
extern struct MapEvent gGondowanCaveRiverEvents[];
extern struct MapEvent gGondowanCaveEvents[];

#endif
