#ifndef ALCHEMY_UNIT_DEBUG_MENU_TEST_TEST_H
#define ALCHEMY_UNIT_DEBUG_MENU_TEST_TEST_H

#include "types.h"
#include "field_scene.h"

u8 *MenuTest_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *MenuTest_GetExits(void);
u8 *MenuTest_GetPlacements(void);
void SceneDialogue_ShowMessageAndWait(s32 arg0);
void CommandTable_RunDirectionalInput(s32 x, s32 cnt);
void SceneState_ApplyBlockC9b(void);
void SceneState_ApplyBlockCc6(void);
void SceneState_ApplyBlockCf1(void);
void SceneState_ApplyBlockD21(void);
void SceneState_ApplyBlockD4c(void);
void SceneState_ApplyBlockD77(void);
void SceneState_ApplyBlockDa2(void);
void DebugMenu_RunMessageTest(void);
void SceneState_ApplyOne(void);
void SceneState_NoOp(void);
void SceneState_QueryTwoValues(void);
void SceneState_ApplyZero(void);
void CommandTable_NoOpCallback(void);
void SceneState_SetRecordFlag53(void);
void DebugMenu_GiveItemToParty(void);
s32 MenuTest_GetEvents(void);
void MenuTest_RaisePartyLevelTarget(void);
void FieldScene_GrantItemListToSlots(void);
void CommandTable_ConfigureCommandGroups(void);
void FieldScene_ApplySlotOffsetsAndFlags(void);
void FieldScene_AssignCodeSetAToSlots(void);
void DebugMenu_SelectItem(void);
void SceneState_RunCall1c00(void);
void FieldScene_AssignCodeSetBToSlots(void);
s32 CommandTable_ConfigureCommandList(void);
s32 SceneState_GetFarResult2384(void);
s32 DebugMenu_SelectCharacter(void);
s32 SceneState_GetFarResult2418(s32 unit_id);
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
