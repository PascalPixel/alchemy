#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_PSYNERGY_STONE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_PSYNERGY_STONE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct MapWarpDest gSolSanctumPsynergyStoneWarpDests[];
extern u32 gSolSanctumPsynergyStoneWarps[];
extern struct ObjectEventTemplate
    gSolSanctumPsynergyStoneObjectEventTemplates[];
extern struct MapEvent gSolSanctumPsynergyStoneEvents[];

#endif
