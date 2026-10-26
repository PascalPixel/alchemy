#ifndef ALCHEMY_UNIT_MAP_ROCK_ROOMS_H
#define ALCHEMY_UNIT_MAP_ROCK_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

extern const struct MapWarpDest gInteriorWarpDests[];
extern const u32 gInteriorWarps[];
extern const struct ObjectEventTemplate gInteriorObjectEventTemplates[];
extern const struct MapEvent gInteriorSealedEvents[];
extern const struct MapEvent gInteriorReopenedEvents[];
extern const struct MapEvent gGateToSuhallaEvents[];
const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);

#endif
