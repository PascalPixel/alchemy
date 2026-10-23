#ifndef ALCHEMY_UNIT_DEBUG_ROOMS_H
#define ALCHEMY_UNIT_DEBUG_ROOMS_H

#include "types.h"
#include "map_tables.h"

u8 *TestRooms_GetWarpDests(void);
s32 MapData_ReturnZero(void);
u8 *TestRooms_GetWarps(void);
u8 *TestRooms_GetObjectEventTemplates(void);
u8 *TestRooms_GetEvents(void);
s32 Map_RunEmpty(void);
extern u32 gTestRooms_WarpDestsHead[];
extern struct MapWarpDest gTestRooms_WarpDests[];
extern u32 gTestRooms_Warps[];
extern struct ObjectEventTemplate gTestRooms_ObjectEventTemplates[];
extern struct MapEvent gTestRooms_Events[];

#endif
