#ifndef ALCHEMY_UNIT_MAP_SUHALLA_INTERIORS_H
#define ALCHEMY_UNIT_MAP_SUHALLA_INTERIORS_H

#include "types.h"
#include "map_tables.h"
#include "field_runtime.h"

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct MapWarpDest gSuhallaInteriorsWarpDests[];
extern struct MapWarpRegion gSuhallaInteriorsRegions[];
extern u32 gSuhallaInteriorsWarps[];
extern struct ObjectEventTemplate gSuhallaInteriorsObjectEventTemplates[];
extern struct ObjectEventTemplate
    gSuhallaInteriorsObjectEventTemplatesAfterAudience[];
extern struct MapEvent gSuhallaInteriorsEvents[];
extern struct MapEvent gSuhallaInteriorsEventsAfterAudience[];

#endif
