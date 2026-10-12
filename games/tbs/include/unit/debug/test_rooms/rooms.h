#ifndef ALCHEMY_UNIT_DEBUG_TEST_ROOMS_ROOMS_H
#define ALCHEMY_UNIT_DEBUG_TEST_ROOMS_ROOMS_H

#include "types.h"
#include "field_scene.h"

u8 *TestRooms_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *TestRooms_GetExits(void);
u8 *TestRooms_GetPlacements(void);
u8 *TestRooms_GetEvents(void);
s32 Scene_RunEmptyScene(void);
extern u32 TestRooms_EntrancesHead[];
extern struct SceneEntrance TestRooms_Entrances[];
extern u32 TestRooms_Exits[];
extern struct ScenePlacement TestRooms_Placements[];
extern struct SceneEvent TestRooms_Events[];

#endif
