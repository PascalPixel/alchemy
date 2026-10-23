#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_PSYNERGY_STONE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_PSYNERGY_STONE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct MapWarpDest gSolSanctumPsynergyStoneWarpDests[];
extern u32 gSolSanctumPsynergyStoneWarps[];
extern struct ObjectEventTemplate
    gSolSanctumPsynergyStoneObjectEventTemplates[];
extern struct MapEvent gSolSanctumPsynergyStoneEvents[];

#endif
