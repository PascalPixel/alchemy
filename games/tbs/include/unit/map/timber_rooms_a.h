#ifndef ALCHEMY_UNIT_MAP_TIMBER_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_TIMBER_ROOMS_A_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct MapWarpDest gTimberRoomsAWarpDests[];
extern struct MapWarpRegion gTimberRoomsARegions[];
extern u32 gTimberRoomsAWarps[];
extern struct ObjectEventTemplate gTimberRoomsAObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTimberRoomsAObjectEventTemplatesAfterAudience[];
extern struct MapEvent gTimberRoomsAEvents[];
extern struct MapEvent gTimberRoomsAEventsAfterAudience[];

#endif
