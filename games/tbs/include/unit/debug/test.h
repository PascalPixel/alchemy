#ifndef ALCHEMY_UNIT_DEBUG_TEST_H
#define ALCHEMY_UNIT_DEBUG_TEST_H

#include "types.h"
#include "map_tables.h"

u8 *MenuTest_GetWarpDests(void);
s32 MapData_ReturnZero(void);
u8 *MenuTest_GetWarps(void);
u8 *MenuTest_GetObjectEventTemplates(void);
s32 MenuTest_GetEvents(void);
s32 CommandTable_ConfigureCommandList(void);
extern u32 gDebugItemPromptHead[];
extern char gDebugItemPrompt[12];
extern char gDebugItemCapacityLabel[24];
extern char gDebugItemFullLabel[20];
extern struct MapWarpDest gMenuTest_WarpDests[];
extern u32 gMenuTest_Warps[];
extern struct ObjectEventTemplate gMenuTest_ObjectEventTemplates[];
extern struct MapEvent gMenuTest_Events[];
extern s32 gMenuTest_SlotOffsets;
extern s32 gMenuTest_FillLetter;

#endif
