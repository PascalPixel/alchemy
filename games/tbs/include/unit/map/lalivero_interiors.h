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
extern struct MapWarpRegion gLaliveroInteriorsA_Regions[];
extern struct MapWarpRegion gLaliveroInteriorsB_Regions[];
extern u32 gLaliveroInteriors_Warps[];
extern struct ObjectEventTemplate gLaliveroInteriorsA_ObjectEventTemplates[];
extern struct ObjectEventTemplate gLaliveroInteriorsB_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gLaliveroInteriorsB_ObjectEventTemplatesAfterLighthouse[];
extern struct MapEvent gLaliveroInteriorsA_Events[];
extern struct MapEvent gLaliveroInteriorsA_EventsAfterLighthouse[];
extern struct MapEvent gLaliveroInteriorsB_Events[];
extern struct MapEvent gLaliveroInteriorsB_EventsAfterLighthouse[];

#endif
