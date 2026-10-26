#ifndef ALCHEMY_UNIT_MAP_VALE_KRADENS_COTTAGE_H
#define ALCHEMY_UNIT_MAP_VALE_KRADENS_COTTAGE_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeKradensCottage_GetWarpDests(void);
const struct MapWarpRegion *ValeKradensCottage_GetRegions(void);
const u32 *ValeKradensCottage_GetWarps(void);
const struct ObjectEventTemplate *
ValeKradensCottage_GetObjectEventTemplates(void);
const struct MapEvent *ValeKradensCottage_GetEvents(void);
s32 ValeKradensCottage_Initialize(void);
extern s32 gValeKradensCottage_RockslideSurvivorEscapeActions[];
extern s32 gValeKradensCottage_RockslideSurvivorSquashes[];
extern s32 gValeKradensCottage_FaceTargetActions[];
extern struct MapWarpDest gValeKradensCottage_WarpDests[];
extern u32 gValeKradensCottage_Warps[];
extern struct ObjectEventTemplate gValeKradensCottage_ObjectEventTemplates[];
extern struct MapEvent gValeKradensCottage_Events[];
extern struct MapEvent gValeKradensCottage_StormEvents[];
extern struct MapEvent gValeKradensCottage_EventsAfterKalay[];
extern u16 gValeKradensCottage_DoorOpens[];

#endif
