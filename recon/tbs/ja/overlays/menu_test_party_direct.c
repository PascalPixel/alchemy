/* Draft: Japanese debug party setup with ordinary count calls.
 * 2026-10-01: Complete 216-byte owner; twelve argument-order bytes differ
 * before the first three count calls, including the coins-store boundary.
 * Ordinary TBS compiler and options; no scene placement or credit claimed.
 */
extern int MenuTest_BenchmarkCharacter;
#include "EDITION.H"
#include "../../../../games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/MENU_TEST.H"
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
struct TextRenderWork *Engine_DebugCreateWindow(s32 x, s32 y, s32 width, s32 height, s32 style);
void Engine_DebugRedrawWindow(struct TextRenderWork *window);
void Engine_DebugClearWindow(struct TextRenderWork *window);
void UiText_DrawStringInWindow(u8 *text, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 format, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawResource(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void Engine_DebugDrawItemDetails(struct TextRenderWork *window, s32 item);

extern u8 MsgDebugGotTreasure[];

/* Party members, in party order. */
enum { ROBIN, JERARD, IWAN, MEARI };

s32 CommandTable_ConfigureCommandList(void)
{
    Party_RemoveActiveOwner(5);
    Party_AddActiveOwner(JERARD);
    Party_AddActiveOwner(MEARI);
    Party_AddActiveOwner(IWAN);
    Item_AdjustCounter(5, 1);
    Item_AdjustCounter(5, 1);
    Item_AdjustCounter(5, 1);
    Item_AdjustCounter(6, 1);
    Item_AdjustCounter(6, 1);
    Item_AdjustCounter(7, 1);
    Item_AdjustCounter(106, 1);
    Item_AdjustCounter(108, 1);
    Item_AdjustCounter(109, 1);
    Item_AdjustCounter(113, 1);
    Item_AdjustCounter(123, 1);
    Item_AdjustCounter(130, 1);
    Item_AdjustCounter(140, 1);
    Item_AdjustCounter(151, 1);
#if EDITION_INTERNATIONAL
    /* FAKEMATCH: the first three calls only match through a cast pointer */
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(ROBIN, 50);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(JERARD, 30);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(MEARI, 30);
#else
    gGameState.coins = 777777;
    Party_AdvanceOwnerCountToTarget(ROBIN, 30);
    Party_AdvanceOwnerCountToTarget(JERARD, 30);
    Party_AdvanceOwnerCountToTarget(MEARI, 30);
#endif
    Party_AdvanceOwnerCountToTarget(IWAN, 30);
    Owner_RecalculateStats(ROBIN);
    Owner_RecalculateStats(JERARD);
    Owner_RecalculateStats(MEARI);
    Owner_RecalculateStats(IWAN);
    return 0;
}
