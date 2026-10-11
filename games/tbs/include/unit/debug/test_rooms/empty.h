#ifndef ALCHEMY_UNIT_DEBUG_TEST_ROOMS_EMPTY_H
#define ALCHEMY_UNIT_DEBUG_TEST_ROOMS_EMPTY_H

#include "types.h"

u8 *TestRooms_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *TestRooms_GetExits(void);
u8 *TestRooms_GetPlacements(void);
void State_ApplyValues8And0And0Then30(void);
void State_ApplyValues9And1And0Then44(void);
void State_ApplyValues10And2And0Then58(void);
void State_ApplyValues11And3And0Then6c(void);
u8 *TestRooms_GetEvents(void);
s32 Scene_RunEmptyScene(void);

#endif
