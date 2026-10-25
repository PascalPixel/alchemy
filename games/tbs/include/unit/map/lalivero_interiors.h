#ifndef ALCHEMY_UNIT_MAP_LALIVERO_INTERIORS_H
#define ALCHEMY_UNIT_MAP_LALIVERO_INTERIORS_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"

const struct MapWarpDest *LaliveroInteriors_GetWarpDests(void);
const struct MapWarpRegion *LaliveroInteriors_GetRegions(void);
const u32 *LaliveroInteriors_GetWarps(void);
const struct ObjectEventTemplate *
LaliveroInteriors_GetObjectEventTemplates(void);
const struct MapEvent *LaliveroInteriors_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gLaliveroInteriors_InnWorkerChores[];
extern struct MapWarpDest gLaliveroInteriors_WarpDests[];
extern struct MapWarpRegion gTimberRoomsB_Regions[];
extern struct MapWarpRegion gTimberRoomsC_Regions[];
extern u32 gLaliveroInteriors_Warps[];
extern struct ObjectEventTemplate gTimberRoomsB_ObjectEventTemplates[];
extern struct ObjectEventTemplate gTimberRoomsC_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTimberRoomsC_ObjectEventTemplatesAfterLighthouse[];
extern struct MapEvent gTimberRoomsB_Events[];
extern struct MapEvent gTimberRoomsB_EventsAfterLighthouse[];
extern struct MapEvent gTimberRoomsC_Events[];
extern struct MapEvent gTimberRoomsC_EventsAfterLighthouse[];

#endif
