#ifndef ALCHEMY_UNIT_DEBUG_ROOMS_H
#define ALCHEMY_UNIT_DEBUG_ROOMS_H

#include "types.h"
#include "map_tables.h"

u8 *TestRoom_GetWarpDests(void);
s32 Map_GetWarpRegions(void);
u8 *TestRoom_GetWarps(void);
u8 *TestRoom_GetObjectEventTemplates(void);
u8 *TestRoom_GetEvents(void);
s32 TestRoom_Initialize(void);
extern u32 gTestRoom_WarpDestsHead[];
extern struct MapWarpDest gTestRoom_WarpDests[];
extern u32 gTestRoom_Warps[];
extern struct ObjectEventTemplate gTestRoom_ObjectEventTemplates[];
extern struct MapEvent gTestRoom_Events[];

#endif
