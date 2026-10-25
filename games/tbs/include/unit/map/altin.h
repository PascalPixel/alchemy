#ifndef ALCHEMY_UNIT_MAP_ALTIN_H
#define ALCHEMY_UNIT_MAP_ALTIN_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;
const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gCottageRoomsD_CookScript[];
extern struct MapWarpDest gAltin_NoWarpDests[];
extern struct MapWarpDest gAltin_WarpDests[];
extern struct MapWarpDest gCottageRoomsD_WarpDests[];
extern u32 gAltin_Warps[];
extern struct ObjectEventTemplate gAltin_NoObjectEventTemplates[];
extern struct ObjectEventTemplate gAltin_ObjectEventTemplates[];
extern struct ObjectEventTemplate gCottageRoomsD_ObjectEventTemplates[];
extern struct MapEvent gAltin_NoEvents[];
extern struct MapEvent gAltin_Events[];
extern struct MapEvent gCottageRoomsD_Events[];
extern u16 gAltin_SanctumDoorCells[];
extern u16 gAltin_SouthwestDoorCells[];
extern u16 gAltin_SoutheastDoorCells[];
extern u16 gAltin_EastDoorCells[];
extern u16 gAltin_NorthwestDoorCells[];
extern u16 gAltin_NorthAndWestDoorCells[];
extern u16 gAltin_NortheastDoorCells[];
extern struct MapGridCellAnimation gAltin_Doors[];

#endif
