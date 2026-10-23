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
extern s32 gAltinHouses_CookScript[];
extern struct MapWarpDest gAltinOther_WarpDests[];
extern struct MapWarpDest gAltinVillage_WarpDests[];
extern struct MapWarpDest gAltinHouses_WarpDests[];
extern u32 gAltin_Warps[];
extern struct ObjectEventTemplate gAltinOther_ObjectEventTemplates[];
extern struct ObjectEventTemplate gAltinVillage_ObjectEventTemplates[];
extern struct ObjectEventTemplate gAltinHouses_ObjectEventTemplates[];
extern struct MapEvent gAltinOther_Events[];
extern struct MapEvent gAltinVillage_Events[];
extern struct MapEvent gAltinHouses_Events[];
extern u16 gAltinVillage_SanctumDoorCells[];
extern u16 gAltinVillage_SouthwestDoorCells[];
extern u16 gAltinVillage_SoutheastDoorCells[];
extern u16 gAltinVillage_EastDoorCells[];
extern u16 gAltinVillage_NorthwestDoorCells[];
extern u16 gAltinVillage_NorthAndWestDoorCells[];
extern u16 gAltinVillage_NortheastDoorCells[];
extern struct MapGridCellAnimation gAltinVillage_Doors[];

#endif
