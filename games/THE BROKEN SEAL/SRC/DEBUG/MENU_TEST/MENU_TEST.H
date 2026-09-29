/* Declarations shared by the debug menu test's sources (resource_3ce). */
#ifndef MENU_TEST_H
#define MENU_TEST_H

#include "TYPES.H"


extern s16 Data_02000240[];
extern volatile s32 Data_03001ae8;
/* The shops' welcome lines open their blocks of messages; the menu steps
 * through a block as long as the distance between the first two. */
extern u8 MsgWeaponShopWelcome[];
extern u8 MsgArmorShopWelcome[];
extern u8 MsgItemShopWelcome[];
extern u8 *Data_03001f30[];
extern u8 Data_03001ebc[];
extern u8 MenuTest_SlotValues[];
extern u8 MenuTest_CommandTable[];
extern u8 MenuTest_CommandTableB[];
extern u8 MenuTest_CommandTableC[];
extern u8 MenuTest_SlotOffsets[];

u8 Engine_TaskWait();
u8 UiText_OpenMessageWindow(s32, s32, s32, s32);
s32 UiWork_IsComplete(void);
u8 UiWork_FinalizePendingCore(void);
s32 UiWindow_CreateWithSideObject(s32, s32, s32, s32);
void UiWork_PushValueSlot(s32, s32);
void SceneDialogue_ShowMessageAndWait();
void Engine_DebugFinalizeWindow(s32, s32);
void CommandTable_RunDirectionalInput();
s32 Battle_ApplyPresetItemsAndFlags();
s32 Shop_PickUnitItem();
s32 NameEntry_EditOwnerName();
void UiText_ShowPositionedMessageAndWait();
s32 Owner_RecalculateStats();
s32 Party_AdvanceOwnerCountToTarget();
s32 Inventory_AddItem();
s32 Djinn_AddToOwner();
s32 Djinn_Activate();
s32 Owner_AdjustFirstValue();
s32 Owner_GetState();
s32 Owner_AdjustSecondValue();
s32 Party_RemoveActiveOwner();
s32 Party_AddActiveOwner();
s32 Item_AdjustCounter();
s32 DebugMenu_BrowseIcons(void);
s32 Shop_ConfirmAct(void);

/* Scene state, dialogue and command-table steps for resource_3ce. */

/*
 * Table getter at 0x02000030. The eight-byte owner includes its one pool word
 * at 0x02000034, which holds 0x020093c8; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/*
 * Table getter at 0x0200003c. The eight-byte owner includes its one pool word
 * at 0x02000040, which holds 0x020093f8; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/*
 * Table getter at 0x02000044. The eight-byte owner includes its one pool word
 * at 0x02000048, which holds 0x020093fc; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/* Step by one until the query returns non-zero. The test sits at the bottom of
 * the loop and entry jumps to it. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call instead precomputes a costly constant
 * into a pseudo shared with later uses in the block. A value-returning call
 * sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Each symbol here is the per-site call word the overlay image holds -- one
 * word can serve two sites with different targets -- and the macro keeps the
 * site's own calling form. The names themselves are provisional. */

/* Runs one setup call, then a long table of two-argument calls each passing
 * a slot index (0-3) and an associated code value, and finishes with a few
 * index-only calls. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* The sources call one another across files. */
u8 *SceneData_GetTable93c8(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetTable93f8(void);
u8 *SceneData_GetTable93fc(void);
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
s32 SceneData_GetTable9564(void);
void FieldScene_ApplyTable9684ValueToFourSlots(void);
void FieldScene_GrantItemListToSlots(void);
void CommandTable_ConfigureCommandGroups(void);
void FieldScene_ApplySlotOffsetsAndFlags(void);
void FieldScene_AssignCodeSetAToSlots(void);
void SceneState_RunCall1c00(void);
void FieldScene_AssignCodeSetBToSlots(void);
s32 CommandTable_ConfigureCommandList(void);
s32 SceneState_GetFarResult2384(void);
s32 SceneState_GetFarResult2418(void);

#endif
