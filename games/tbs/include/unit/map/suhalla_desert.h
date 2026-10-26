#ifndef ALCHEMY_UNIT_MAP_SUHALLA_DESERT_H
#define ALCHEMY_UNIT_MAP_SUHALLA_DESERT_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
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
