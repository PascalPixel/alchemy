#ifndef ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H
#define ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H

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
extern const struct MapWarpDest gCaveWarpDests[];
extern const u32 gCaveWarps[];
extern const struct ObjectEventTemplate gCaveNoObjectEventTemplates[];
extern const struct ObjectEventTemplate gCaveObjectEventTemplates[];
extern const struct MapEvent gCaveNoEvents[];
extern const struct MapEvent gCaveEvents[];

#endif
