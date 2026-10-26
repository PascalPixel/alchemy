#ifndef ALCHEMY_UNIT_DEBUG_ROOMS_H
#define ALCHEMY_UNIT_DEBUG_ROOMS_H

#include "types.h"
#include "map_tables.h"

u8 *TestRoom_GetEntrances(void);
s32 Scene_GetExitRegions(void);
u8 *TestRoom_GetExits(void);
u8 *TestRoom_GetObjectEventTemplates(void);
u8 *TestRoom_GetEvents(void);
s32 TestRoom_Initialize(void);
extern u32 gTestRoom_EntrancesHead[];
extern struct SceneEntrance gTestRoom_Entrances[];
extern u32 gTestRoom_Exits[];
extern struct ObjectEventTemplate gTestRoom_ObjectEventTemplates[];
extern struct MapEvent gTestRoom_Events[];

#endif
