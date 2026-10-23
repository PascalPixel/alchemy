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
extern struct MapWarpRegion gLaliveroInteriors_ShopsRegions[];
extern struct MapWarpRegion gLaliveroInteriors_FaransHouseRegions[];
extern u32 gLaliveroInteriors_Warps[];
extern struct ObjectEventTemplate
    gLaliveroInteriors_ShopsObjectEventTemplates[];
extern struct ObjectEventTemplate
    gLaliveroInteriors_FaransHouseObjectEventTemplates[];
extern struct ObjectEventTemplate
    gLaliveroInteriors_FaransHouseObjectEventTemplatesAfterLighthouse[];
extern struct MapEvent gLaliveroInteriors_ShopsEvents[];
extern struct MapEvent gLaliveroInteriors_ShopsEventsAfterLighthouse[];
extern struct MapEvent gLaliveroInteriors_FaransHouseEvents[];
extern struct MapEvent gLaliveroInteriors_FaransHouseEventsAfterLighthouse[];

#endif
