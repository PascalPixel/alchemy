#ifndef ALCHEMY_UNIT_MAP_BILIBIN_INTERIORS_H
#define ALCHEMY_UNIT_MAP_BILIBIN_INTERIORS_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *BilibinInteriors_GetWarpDests(void);
const struct MapWarpRegion *BilibinInteriors_GetRegions(void);
const u32 *BilibinInteriors_GetWarps(void);
const struct ObjectEventTemplate *
BilibinInteriors_GetObjectEventTemplates(void);
const struct MapEvent *BilibinInteriors_GetEvents(void);
s32 BilibinInteriors_Initialize(void);
extern struct MapWarpDest gBilibinInteriorsWarpDests[];
extern u32 gBilibinInteriorsWarps[];
extern struct ObjectEventTemplate gBilibinInteriorsObjectEventTemplates[];
extern struct MapEvent gBilibinInteriorsEvents[];

#endif
