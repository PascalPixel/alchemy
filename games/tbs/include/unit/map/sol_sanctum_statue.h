#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_STATUE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_STATUE_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *SolSanctumStatue_GetWarpDests(void);
const struct MapWarpRegion *SolSanctumStatue_GetRegions(void);
const u32 *SolSanctumStatue_GetWarps(void);
const struct ObjectEventTemplate *
SolSanctumStatue_GetObjectEventTemplates(void);
const struct MapEvent *SolSanctumStatue_GetEvents(void);
s32 SolSanctumStatue_Initialize(void);
extern struct MapWarpDest gSolSanctumStatue_WarpDests[];
extern u32 gSolSanctumStatue_Warps[];
extern struct ObjectEventTemplate gSolSanctumStatue_ObjectEventTemplates[];
extern struct MapEvent gSolSanctumStatue_Events[];
extern s32 gSolSanctumStatue_ScrollPhase;

#endif
