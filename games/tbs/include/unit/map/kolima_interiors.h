#ifndef ALCHEMY_UNIT_MAP_KOLIMA_INTERIORS_H
#define ALCHEMY_UNIT_MAP_KOLIMA_INTERIORS_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *KolimaInteriors_GetWarpDests(void);
const struct MapWarpRegion *KolimaInteriors_GetRegions(void);
const u32 *KolimaInteriors_GetWarps(void);
const struct ObjectEventTemplate *KolimaInteriors_GetObjectEventTemplates(void);
const struct MapEvent *KolimaInteriors_GetEvents(void);
s32 KolimaInteriors_Initialize(void);
extern struct MapWarpDest gKolimaInteriors_WarpDests[];
extern u32 gKolimaInteriors_Warps[];
extern struct ObjectEventTemplate gKolimaInteriors_ObjectEventTemplates[];
extern struct MapEvent gKolimaInteriors_Events[];

#endif
