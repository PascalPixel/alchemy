#ifndef ALCHEMY_UNIT_DEBUG_TEST_H
#define ALCHEMY_UNIT_DEBUG_TEST_H

#include "types.h"
#include "map_tables.h"

u8 *MenuTest_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *MenuTest_GetExits(void);
u8 *MenuTest_GetPlacements(void);
s32 MenuTest_GetEvents(void);
s32 CommandTable_ConfigureCommandList(void);
extern u32 gDebugItemPromptHead[];
extern char gDebugItemPrompt[12];
extern char gDebugItemCapacityLabel[24];
extern char gDebugItemFullLabel[20];
extern struct SceneEntrance gMenuTest_Entrances[];
extern u32 gMenuTest_Exits[];
extern struct ScenePlacement gMenuTest_Placements[];
extern struct SceneEvent gMenuTest_Events[];
extern s32 gMenuTest_SlotOffsets;
extern s32 gMenuTest_FillLetter;

#endif
