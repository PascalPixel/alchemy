#ifndef ALCHEMY_UNIT_DEBUG_MENU_TEST_TEST_H
#define ALCHEMY_UNIT_DEBUG_MENU_TEST_TEST_H

#include "types.h"
#include "field_scene.h"

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
extern struct SceneEntrance MenuTest_Entrances[];
extern u32 MenuTest_Exits[];
extern struct ScenePlacement MenuTest_Placements[];
extern struct SceneEvent MenuTest_Events[];
extern s32 MenuTest_SlotOffsets;
extern s32 MenuTest_BenchmarkCharacter;

#endif
