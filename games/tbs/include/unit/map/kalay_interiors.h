#ifndef ALCHEMY_UNIT_MAP_KALAY_INTERIORS_H
#define ALCHEMY_UNIT_MAP_KALAY_INTERIORS_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *KalayInteriors_GetWarpDests(void);
const struct MapWarpRegion *KalayInteriors_GetRegions(void);
const u32 *KalayInteriors_GetWarps(void);
const struct ObjectEventTemplate *KalayInteriors_GetObjectEventTemplates(void);
const struct MapEvent *KalayInteriors_GetEvents(void);
s32 KalayInteriors_Initialize(void);
extern struct MapWarpDest gKalayInteriors_NoWarpDests[];
extern struct MapWarpDest gKalayInteriors_HouseWarpDests[];
extern struct MapWarpDest gKalayInteriors_SanctumWarpDests[];
extern u32 gKalayInteriors_Warps[];
extern struct ObjectEventTemplate gKalayInteriors_NoObjectEventTemplates[];
extern struct ObjectEventTemplate gKalayInteriors_HouseObjectEventTemplates[];
extern struct ObjectEventTemplate gKalayInteriors_InnObjectEventTemplates[];
extern struct ObjectEventTemplate gKalayInteriors_SanctumObjectEventTemplates[];
extern struct MapEvent gKalayInteriors_NoEvents[];
extern struct MapEvent gKalayInteriors_HouseEvents[];
extern struct MapEvent gKalayInteriors_InnEvents[];
extern struct MapEvent gKalayInteriors_SanctumEvents[];
extern u16 gKalayInteriors_DoorCellSteps[];
extern struct MapGridCellAnimation gKalayInteriors_DoorCells[];

#endif
