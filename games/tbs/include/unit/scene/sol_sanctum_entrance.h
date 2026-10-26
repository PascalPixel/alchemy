#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *SolSanctumEntrance_GetWarpDests(void);
const struct MapWarpRegion *SolSanctumEntrance_GetRegions(void);
const u32 *SolSanctumEntrance_GetWarps(void);
const struct ObjectEventTemplate *
SolSanctumEntrance_GetObjectEventTemplates(void);
const struct MapEvent *SolSanctumEntrance_GetEvents(void);
s32 SolSanctumEntrance_Initialize(void);
extern struct MapWarpDest gSolSanctumEntrance_FallbackWarpDests[];
extern struct MapWarpDest gSolEntrance_WarpDests[];
extern struct MapWarpDest gSolSanctumReliefs_WarpDests[];
extern u32 gSolSanctumEntrance_Warps[];
extern struct ObjectEventTemplate
    gSolSanctumEntrance_FallbackObjectEventTemplates[];
extern struct ObjectEventTemplate gSolSanctumReliefs_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gSolSanctumReliefs_FirstReliefRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gSolSanctumReliefs_SecondReliefRoomObjectEventTemplates[];
extern struct ObjectEventTemplate gSolEntrance_ObjectEventTemplates[];
extern struct MapEvent gSolSanctumEntrance_FallbackEvents[];
extern struct MapEvent gSolEntrance_Events[];
extern struct MapEvent gSolSanctumReliefs_Events[];
extern struct MapEvent gSolSanctumReliefs_FirstReliefRoomEvents[];
extern struct MapEvent gSolSanctumReliefs_SecondReliefRoomEvents[];
extern s32 gSolSanctumPushSteps[];

#endif
