#ifndef ALCHEMY_UNIT_MAP_SUHALLA_H
#define ALCHEMY_UNIT_MAP_SUHALLA_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct MapWarpDest gSuhallaWarpDests[];
extern u32 gSuhallaWarps[];
extern struct ObjectEventTemplate gSuhallaObjectEventTemplates[];
extern struct ObjectEventTemplate gSuhallaObjectEventTemplatesLater[];
extern s16 gSuhallaDoorCells[][2];
extern u16 gSuhallaDoorOpenSteps[];
extern u16 gSuhallaWideDoorOpenSteps[];
extern struct MapEvent gSuhallaEvents[];
extern struct MapEvent gSuhallaEventsLater[];

#endif
