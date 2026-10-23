#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *SolSanctumEruption_GetWarpDests(void);
const struct MapWarpRegion *SolSanctumEruption_GetRegions(void);
const u32 *SolSanctumEruption_GetWarps(void);
const struct ObjectEventTemplate *
SolSanctumEruption_GetObjectEventTemplates(void);
const struct MapEvent *SolSanctumEruption_GetEvents(void);
s32 SolSanctumEruption_Initialize(void);
extern s32 gEruption_ArcOriginsHead[];
extern s32 gEruption_ArcOrigins[];
extern struct MapWarpDest gSolSanctumEruption_WarpDests[];
extern u32 gSolSanctumEruption_Warps[];
extern struct ObjectEventTemplate gSolSanctumEruption_ObjectEventTemplates[];
extern struct MapEvent gSolSanctumEruption_Events[];
extern u16 gSolSanctumEruption_UnusedStarCells[];
extern s32 gEruption_EmberScript[];
extern u8 gSolSanctum_RingDrift[][3];
extern u8 gSolSanctum_RingSwing[][3];
extern s8 gSolSanctum_RingDirection[][3];
extern s32 gSolSanctumRing_MaxScales[];
extern s32 gSolSanctumRing_ScaleSteps[];

#endif
