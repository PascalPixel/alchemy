#ifndef ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H
#define ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern const struct MapWarpDest gCaveWarpDests[];
extern const u32 gCaveWarps[];
extern const struct ObjectEventTemplate gCaveNoObjectEventTemplates[];
extern const struct ObjectEventTemplate gCaveObjectEventTemplates[];
extern const struct MapEvent gCaveNoEvents[];
extern const struct MapEvent gCaveEvents[];

#endif
