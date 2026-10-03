/* Draft: Japanese debug command source alternative.
 * 2026-10-01: Compiled owner 664 bytes; clear/fill register allocation or scheduling differs from the 664-byte Japanese owner.
 * This reduced attempt includes its current source declaration context.
 * 2026-10-03: HP declaration now comes from OWNERVAL.H; body unchanged.
 * The scorer has no listing label for CommandTable_ConfigureCommandGroups.
 * Current compilation is blocked by the existing private
 * UiText_DrawCharacterAtOffset declaration conflicting with
 * TEXT_RENDER_RUNTIME.H. The earlier 664-byte trial is retained; no new
 * byte proof or scene placement is claimed by this declaration correction.
 * Ordinary TBS compiler and options; no scene placement or credit claimed.
 */
extern struct { int letter; } MenuTest_BenchmarkCharacter;
#include "EDITION.H"
/* Declarations shared by the debug menu test's sources (resource_3ce). */
#ifndef MENU_TEST_H
#define MENU_TEST_H

#include "TYPES.H"
#include "OWNER_STATE.H"
#include "OWNERVAL.H"
#include "GAME_STATE.H"

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
void Owner_RecalculateStats();
void Party_AdvanceOwnerCountToTarget();
s32 Inventory_AddItem();
s32 Djinn_AddToOwner();
s32 Djinn_Activate();
s32 Owner_AdjustSecondValue();
s32 Party_RemoveActiveOwner();
s32 Party_AddActiveOwner();
s32 Item_AdjustCounter();
s32 DebugMenu_BrowseIcons(void);
s32 Shop_ConfirmAct(void);
s32 Menu_OpenCharacterSelector(void);

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

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Each symbol here is the per-site call word the overlay image holds -- one
 * word can serve two sites with different targets -- and the macro keeps the
 * site's own calling form. The names themselves are provisional. */

/* Runs one setup call, then a long table of two-argument calls each passing
 * a slot index (0-3) and an associated code value, and finishes with a few
 * index-only calls. */
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
s32 DebugMenu_SelectCharacter(void);

#endif

#include "CALL.H"
#include "TYPES.H"
#include "INVENTORY.H"
#include "DMA.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "ITEM.H"
#include "TBS_EDITION.H"
#include "text/MSG_IDS.H"

TEXT_MESSAGE_ENUM(MsgBattleAdditionalPsynergyHelp);
TEXT_MESSAGE_ENUM(MsgHaidiaComeOnHurry);

void Event_SetValue1d8(s16 message);
void BattleEv_RunWait(s32 mode, s32 frames);
s32 PartyInventory_GiveItem(s32 item);

extern u8 MsgSanctumWelcome[];
extern u8 MsgWarriorShopWelcome[];
extern u8 MsgWarriorArmorShopWelcome[];
extern u8 MsgWarriorItemShopWelcome[];
extern u8 MsgDebugBodyTornApart[];
extern u8 MsgDebugGotDjinni[];
extern u8 MsgDebugGotItem[];
extern u8 MsgDebugGotSturdyEquipment[];
extern u8 MsgDebugWentLevel[];

extern u8 MsgItemName[];
extern u8 MsgItemPlainName[];
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_AudioPlayCue(s32 cue);
extern u8 gDebugItemPrompt[];
extern u8 gDebugItemCapacityLabel[];
extern u8 gDebugItemFullLabel[];
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
struct RenderInput *Engine_DebugCreateWindow(s32 x, s32 y, s32 width, s32 height, s32 style);
void Engine_DebugRedrawWindow(struct RenderInput *window);
void Engine_DebugClearWindow(struct RenderInput *window);
void UiText_DrawStringInWindow(u8 *text, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 format, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 text, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawResource(s32 text, struct RenderInput *window, s32 x, s32 y);
void Engine_DebugDrawItemDetails(struct RenderInput *window, s32 item);

extern u8 MsgDebugGotTreasure[];

/* Party members, in party order. */
enum { ROBIN, JERARD, IWAN, MEARI };


void CommandTable_ConfigureCommandGroups(void)
{
    u8 buf[256];
#if !EDITION_INTERNATIONAL
    s32 i;
    s32 j;
    u8 *p;

    for (i = 0; i < 16; i++) {
        for (j = 255; j >= 0; j--) {
            buf[j] = 0;
        }
        for (j = 0; j < i + 32; j++) {
            buf[j] = MenuTest_BenchmarkCharacter.letter + 1;
        }
        (MenuTest_BenchmarkCharacter.letter)++;
        if (MenuTest_BenchmarkCharacter.letter > 96) {
            MenuTest_BenchmarkCharacter.letter = 65;
        }
    }
#endif
    UiText_ShowPositionedMessageAndWait((s32)MsgDebugGotDjinni, 1);
    Djinn_AddToOwner(0, 0, 0);
    Djinn_AddToOwner(0, 0, 1);
    Djinn_AddToOwner(0, 0, 2);
    Djinn_AddToOwner(0, 0, 3);
    Djinn_AddToOwner(0, 0, 4);
    Djinn_AddToOwner(0, 0, 5);
    Djinn_AddToOwner(0, 0, 6);
    Djinn_Activate(0, 0, 0);
    Djinn_Activate(0, 0, 1);
    Djinn_Activate(0, 0, 2);
    Djinn_Activate(0, 0, 3);
    Djinn_Activate(0, 0, 4);
    Djinn_Activate(0, 0, 5);
    Djinn_Activate(0, 0, 6);
    Djinn_AddToOwner(1, 2, 0);
    Djinn_AddToOwner(1, 2, 1);
    Djinn_AddToOwner(1, 2, 2);
    Djinn_AddToOwner(1, 2, 3);
    Djinn_AddToOwner(1, 2, 4);
    Djinn_AddToOwner(1, 2, 5);
    Djinn_AddToOwner(1, 2, 6);
    Djinn_Activate(1, 2, 0);
    Djinn_Activate(1, 2, 1);
    Djinn_Activate(1, 2, 2);
    Djinn_Activate(1, 2, 3);
    Djinn_Activate(1, 2, 4);
    Djinn_Activate(1, 2, 5);
    Djinn_Activate(1, 2, 6);
    Djinn_AddToOwner(3, 1, 0);
    Djinn_AddToOwner(3, 1, 1);
    Djinn_AddToOwner(3, 1, 2);
    Djinn_AddToOwner(3, 1, 3);
    Djinn_AddToOwner(3, 1, 4);
    Djinn_AddToOwner(3, 1, 5);
    Djinn_AddToOwner(3, 1, 6);
    Djinn_Activate(3, 1, 0);
    Djinn_Activate(3, 1, 1);
    Djinn_Activate(3, 1, 2);
    Djinn_Activate(3, 1, 3);
    Djinn_Activate(3, 1, 4);
    Djinn_Activate(3, 1, 5);
    Djinn_Activate(3, 1, 6);
    Djinn_AddToOwner(2, 3, 0);
    Djinn_AddToOwner(2, 3, 1);
    Djinn_AddToOwner(2, 3, 2);
    Djinn_AddToOwner(2, 3, 3);
    Djinn_AddToOwner(2, 3, 4);
    Djinn_AddToOwner(2, 3, 5);
    Djinn_Activate(2, 3, 0);
    Djinn_Activate(2, 3, 1);
    Djinn_Activate(2, 3, 2);
    Djinn_Activate(2, 3, 3);
    Djinn_Activate(2, 3, 4);
    Djinn_Activate(2, 3, 5);
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(3);
    Owner_RecalculateStats(2);
}
