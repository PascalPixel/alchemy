/* Draft: Japanese debug command source alternative.
 * 2026-10-01: Compiled owner 664 bytes; clear/fill register allocation or scheduling differs from the 664-byte Japanese owner.
 * This reduced attempt includes its current source declaration context.
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
    s32 words[64];
    u8 *buf;
#if !EDITION_INTERNATIONAL
    s32 i;
    s32 j;
    u8 *p;

    buf = (u8 *)words;
    for (i = 0; i < 16; i++) {
        for (j = 255; j >= 0; j--) {
            buf[j] = 0;
        }
        for (j = 0; j < i + 32; j++) {
            buf[j] = MenuTest_BenchmarkCharacter + 1;
        }
        (MenuTest_BenchmarkCharacter)++;
        if (MenuTest_BenchmarkCharacter > 96) {
            MenuTest_BenchmarkCharacter = 65;
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
