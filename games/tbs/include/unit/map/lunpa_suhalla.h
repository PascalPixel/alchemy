#ifndef ALCHEMY_UNIT_MAP_LUNPA_SUHALLA_H
#define ALCHEMY_UNIT_MAP_LUNPA_SUHALLA_H

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
const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);

#endif
