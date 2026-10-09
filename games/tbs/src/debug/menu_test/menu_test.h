#ifndef MENU_TEST_H
#define MENU_TEST_H

#include "types.h"
#include "owner_state.h"
#include "ownerval.h"
#include "game_state.h"
#include "fx_scene.h"
#include "fieldobj.h"
#include "text/msg_ids.h"

#include "character.h"
#define UnitEpAdd ((s32 (*)())UnitEpAdd)

extern volatile s32			 gKeysHeld;
extern struct BattleFxScene *gEffectWork;
extern u8					 MenuTest_Events[];
extern u8					 MenuTest_Entrances[];
extern u8					 MenuTest_Exits[];
extern u8					 MenuTest_Placements[];
extern u8					 MenuTest_SlotOffsets[];
extern s32					 MenuTest_BenchmarkCharacter;

u8	 Engine_TaskWait();
u8	 UiText_OpenMessageWindow(s32, s32, s32, s32);
s32	 UiWork_IsComplete(void);
u8	 UiWork_FinalizePendingCore(void);
s32	 UiWindow_CreateWithSideObject(s32, s32, s32, s32);
void UiWork_PushValueSlot(s32, s32);
void SceneDialogue_ShowMessageAndWait();
void Engine_DebugFinalizeWindow(s32, s32);
void CommandTable_RunDirectionalInput();
s32	 Battle_ApplyPresetItemsAndFlags();
s32	 Shop_PickUnitItem();
s32	 NameEntry_EditOwnerName();
void UiText_ShowPositionedMessageAndWait();
void UnitCalc();
void Party_AdvanceOwnerCountToTarget();
s32	 Inventory_AddItem();
s32	 Djinn_AddToOwner();
s32	 Djinn_Activate();
s32	 Party_RemoveActiveOwner();
s32	 Party_AddActiveOwner();
s32	 Item_AdjustCounter();
s32	 DebugMenu_BrowseIcons(void);
s32	 Shop_ConfirmAct(void);
s32	 Menu_OpenCharacterSelector(void);

u8	*MenuTest_GetEntrances(void);
s32	 SceneData_ReturnZero(void);
u8	*MenuTest_GetExits(void);
u8	*MenuTest_GetPlacements(void);
void SceneState_ApplyBlockC9b(void);
void SceneState_ApplyBlockCc6(void);
void SceneState_ApplyBlockCf1(void);
void SceneState_ApplyBlockD4c(void);
void SceneState_ApplyOne(void);
void SceneState_NoOp(void);
void SceneState_QueryTwoValues(void);
void SceneState_ApplyZero(void);
void CommandTable_NoOpCallback(void);
void SceneState_SetRecordFlag53(void);
s32	 MenuTest_GetEvents(void);
void MenuTest_RaisePartyLevelTarget(void);
void FieldScene_GrantItemListToSlots(void);
void CommandTable_ConfigureCommandGroups(void);
void FieldScene_ApplySlotOffsetsAndFlags(void);
void FieldScene_AssignCodeSetAToSlots(void);
void SceneState_RunCall1c00(void);
void FieldScene_AssignCodeSetBToSlots(void);
s32	 CommandTable_ConfigureCommandList(void);
s32	 SceneState_GetFarResult2384(void);
s32	 SceneState_GetFarResult2418(void);
s32	 DebugMenu_SelectCharacter(void);

#endif
