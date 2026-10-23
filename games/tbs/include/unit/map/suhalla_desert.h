#ifndef ALCHEMY_UNIT_MAP_SUHALLA_DESERT_H
#define ALCHEMY_UNIT_MAP_SUHALLA_DESERT_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gShortEffectScript[];
extern s32 gMediumEffectScript[];
extern s32 gLongEffectScript[];
extern const s32 *gEffectScripts[];
extern s32 gSuhallaDesertStormScript[];
extern s32 gSuhallaDesertFourthStormScript[];
extern struct MapWarpDest gSuhallaDesertWarpDestsOther[];
extern struct MapWarpDest gSuhallaDesertNorthWarpDests[];
extern struct MapWarpDest gSuhallaDesertSouthWarpDests[];
extern struct MapWarpDest gSuhallaDesertCavesWarpDests[];
extern u32 gSuhallaDesertWarps[];
extern struct ObjectEventTemplate gSuhallaDesertObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gSuhallaDesertNorthObjectEventTemplates[];
extern struct ObjectEventTemplate gSuhallaDesertSouthObjectEventTemplates[];
extern struct ObjectEventTemplate gSuhallaDesertCavesObjectEventTemplates[];
extern s32 gSuhallaDesertChaseScript[];
extern struct MapEvent gSuhallaDesertEventsOther[];
extern struct MapEvent gSuhallaDesertCavesEvents[];

#endif
