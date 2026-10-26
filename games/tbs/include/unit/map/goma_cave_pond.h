#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_POND_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_POND_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct MapWarpDest gGomaCavePondWarpDests[];
extern u32 gGomaCavePondWarps[];
extern struct ObjectEventTemplate gGomaCavePondObjectEventTemplates[];
extern struct MapEvent gGomaCavePondEvents[];

#endif
