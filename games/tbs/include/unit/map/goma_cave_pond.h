#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_POND_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_POND_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct MapWarpDest gGomaCavePondWarpDests[];
extern u32 gGomaCavePondWarps[];
extern struct ObjectEventTemplate gGomaCavePondObjectEventTemplates[];
extern struct MapEvent gGomaCavePondEvents[];

#endif
