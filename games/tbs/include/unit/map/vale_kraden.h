#ifndef ALCHEMY_UNIT_MAP_VALE_KRADEN_H
#define ALCHEMY_UNIT_MAP_VALE_KRADEN_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeKraden_GetWarpDests(void);
const struct MapWarpRegion *ValeKraden_GetRegions(void);
const u32 *ValeKraden_GetWarps(void);
const struct ObjectEventTemplate *ValeKraden_GetObjectEventTemplates(void);
const struct MapEvent *ValeKraden_GetEvents(void);
s32 ValeKraden_Initialize(void);
extern s32 gValeKraden_RockslideSurvivorEscapeActions[];
extern s32 gValeKraden_RockslideSurvivorSquashes[];
extern s32 gValeKraden_FaceTargetActions[];
extern struct MapWarpDest gValeKraden_WarpDests[];
extern u32 gValeKraden_Warps[];
extern struct ObjectEventTemplate gValeKraden_ObjectEventTemplates[];
extern struct MapEvent gValeKraden_Events[];
extern struct MapEvent gValeKraden_StormEvents[];
extern struct MapEvent gValeKraden_EventsAfterKalay[];
extern u16 gValeKraden_GateCells[];

#endif
