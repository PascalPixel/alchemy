#ifndef ALCHEMY_UNIT_DEBUG_ROOMS_H
#define ALCHEMY_UNIT_DEBUG_ROOMS_H

#include "types.h"
#include "map_tables.h"

u8 *TestRooms_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *TestRooms_GetExits(void);
u8 *TestRooms_GetPlacements(void);
u8 *TestRooms_GetEvents(void);
s32 Scene_RunEmptyScene(void);
extern u32 gTestRooms_EntrancesHead[];
extern struct SceneEntrance gTestRooms_Entrances[];
extern u32 gTestRooms_Exits[];
extern struct ScenePlacement gTestRooms_Placements[];
extern struct SceneEvent gTestRooms_Events[];

#endif
