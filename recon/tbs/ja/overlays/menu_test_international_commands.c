/* Draft: Japanese debug-room source.
 * 2026-10-01: This international model produced a 5,772-byte Japanese scene,
 * 416 bytes longer than its own image. The Japanese scene omits the directional
 * message group, adds a buffer benchmark and character selector, sets 777,777
 * coins and levels all four owners to 30. This attempt retains the earlier
 * international behavior.
 * Compiles with ordinary TBS flags; no source selection or credit is claimed.
 */
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

u8 *SceneData_GetTable93c8(void)
{
    return MenuTest_CommandTable;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable93f8(void)
{
    return MenuTest_CommandTableB;
}

u8 *SceneData_GetTable93fc(void)
{
    return MenuTest_CommandTableC;
}

void SceneDialogue_ShowMessageAndWait(s32 arg0)
{
    UiWork_FinalizePendingCore();
    UiText_OpenMessageWindow(arg0, 5, 0, 0x22);
    while (UiWork_IsComplete() == 0) {
        Engine_TaskWait(1);
    }
    Engine_TaskWait(1);
}

void CommandTable_RunDirectionalInput(s32 x, s32 cnt)
{
    s16 *tbl = Data_02000240;
    volatile s32 *key;
    s32 token;
    s32 i;

    *(u8 *)&tbl[262] = 2;
    token = UiWindow_CreateWithSideObject(125, 0, 0, 0);
    for (i = 0; i < cnt; i++) {
        key = &Data_03001ae8;
        UiWork_PushValueSlot(1, 1);
        UiWork_PushValueSlot(141, 2);
        UiWork_PushValueSlot(0x1e240, 5);
        SceneDialogue_ShowMessageAndWait(x);
        goto test;
retry:
        if (*key != 0) {
            goto next;
        }
        Engine_TaskWait(1);
test:
        if ((*key & 2) != 0) {
            goto end;
        }
        if ((*key & 1) != 0) {
            goto inc;
        }
        if ((*key & 0x80) == 0) {
            goto other;
        }
inc:
        x++;
        goto next;
other:
        if ((*key & 0x40) != 0) {
            x--;
            goto next;
        }
        goto retry;
next:;
    }
end:
    UiWork_FinalizePendingCore();
    Engine_DebugFinalizeWindow(token, 2);
}

void SceneState_ApplyBlockC9b(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWeaponShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockCc6(void)
{
    CommandTable_RunDirectionalInput((s32)MsgArmorShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockCf1(void)
{
    CommandTable_RunDirectionalInput((s32)MsgItemShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockD21(void)
{
    CommandTable_RunDirectionalInput((s32)MsgSanctumWelcome, (s32)MsgWarriorShopWelcome - (s32)MsgSanctumWelcome);
}

void SceneState_ApplyBlockD4c(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockD77(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorArmorShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockDa2(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorItemShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
void DebugMenu_RunMessageTest(void)
{
    Event_SetValue1d8(MsgBattleAdditionalPsynergyHelp);
    Event_SetValue1d8(MsgHaidiaComeOnHurry);
    BattleEv_RunWait(-1, 0);
}
#endif

void SceneState_ApplyOne(void)
{
    Battle_ApplyPresetItemsAndFlags(1);
}

void SceneState_NoOp(void)
{
}

void SceneState_QueryTwoValues(void)
{
    s32 a;
    s32 b;
    Shop_PickUnitItem(&a, &b);
}

void SceneState_ApplyZero(void)
{
    NameEntry_EditOwnerName(0);
}

void CommandTable_NoOpCallback(void)
{
}

void SceneState_SetRecordFlag53(void)
{
    Data_03001f30[0][0x35] = 1;
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
void DebugMenu_GiveItemToParty(void)
{
    /* FAKEMATCH: the obsolete second-argument zero must use r1 before the one-argument item API. */
    register s32 unused asm("r1") = 0;

    /* FAKEMATCH: keeping the unused r1 value live emits the original caller setup; a plain call omits it. */
    asm("" : : "r"(unused));
    PartyInventory_GiveItem(181);
}
#endif

s32 SceneData_GetTable9564(void)
{
    return (s32)MenuTest_SlotValues;
}

void FieldScene_ApplyTable9684ValueToFourSlots(void)
{
    s32 *p;

    UiText_ShowPositionedMessageAndWait((s32)MsgDebugWentLevel, 1);
    p = (s32 *)MenuTest_SlotOffsets;
    Party_AdvanceOwnerCountToTarget(0, *p);
    Party_AdvanceOwnerCountToTarget(1, *p);
    Party_AdvanceOwnerCountToTarget(3, *p);
    Party_AdvanceOwnerCountToTarget(2, *p);
    *p += 10;
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(3);
    Owner_RecalculateStats(2);
}

void FieldScene_GrantItemListToSlots(void)
{
    u32 tmp;
    s32 slot;

    UiText_ShowPositionedMessageAndWait((s32)MsgDebugGotItem, 1);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 187);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 180);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 181);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 182);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(0, 183);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 186);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(1, 187);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 188);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 189);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(2, 236);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 191);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 192);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 193);
    Inventory_AddItem(3, 194);
    Inventory_AddItem(3, 194);
    Inventory_AddItem(3, 194);
    Inventory_AddItem(3, 194);
    Inventory_AddItem(3, 194);
    Inventory_AddItem(3, 194);
    Inventory_AddItem(3, 194);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 195);
    Inventory_AddItem(3, 196);
    Inventory_AddItem(3, 196);
    Inventory_AddItem(3, 196);
    Inventory_AddItem(3, 196);
    Inventory_AddItem(3, 196);
    Inventory_AddItem(3, 196);
    Inventory_AddItem(3, 196);
    Inventory_AddItem(3, 196);
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(3);
    Owner_RecalculateStats(2);
}

void CommandTable_ConfigureCommandGroups(void)
{
    u8 buf[256];
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

void FieldScene_ApplySlotOffsetsAndFlags(void)
{
    u32 i;
    u8 *record;

    UiText_ShowPositionedMessageAndWait((s32)MsgDebugBodyTornApart, 1);
    Value2(Owner_AdjustFirstValue, 0, -100);
    Value2(Owner_AdjustFirstValue, 1, -100);
    Owner_AdjustFirstValue(2, -33);
    Owner_AdjustFirstValue(3, -100);
    Owner_AdjustSecondValue(0, -50);
    Owner_AdjustSecondValue(1, -40);
    Owner_AdjustSecondValue(2, -35);
    Owner_AdjustSecondValue(3, -20);
    record = Owner_GetState(0);
    record[0x131] = 1;
    record += 0x140;
    *record = 1;
    record = Owner_GetState(1);
    record[0x130] = 1;
    record[0x131] = 2;
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(3);
    Owner_RecalculateStats(2);
}

void FieldScene_AssignCodeSetAToSlots(void)
{
    UiText_ShowPositionedMessageAndWait((s32)MsgDebugGotSturdyEquipment, 1);
    Inventory_AddItem(0, 85);
    Inventory_AddItem(0, 84);
    Inventory_AddItem(0, 124);
    Inventory_AddItem(0, 123);
    Inventory_AddItem(0, 9);
    Inventory_AddItem(0, 11);
    Inventory_AddItem(0, 27);
    Inventory_AddItem(0, 26);
    Inventory_AddItem(1, 38);
    Inventory_AddItem(1, 37);
    Inventory_AddItem(1, 50);
    Inventory_AddItem(1, 49);
    Inventory_AddItem(1, 83);
    Inventory_AddItem(1, 82);
    Inventory_AddItem(1, 134);
    Inventory_AddItem(1, 133);
    Inventory_AddItem(1, 152);
    Inventory_AddItem(2, 64);
    Inventory_AddItem(2, 65);
    Inventory_AddItem(2, 98);
    Inventory_AddItem(2, 97);
    Inventory_AddItem(2, 124);
    Inventory_AddItem(2, 131);
    Inventory_AddItem(2, 141);
    Inventory_AddItem(2, 163);
    Inventory_AddItem(3, 61);
    Inventory_AddItem(3, 63);
    Inventory_AddItem(3, 96);
    Inventory_AddItem(3, 95);
    Inventory_AddItem(3, 113);
    Inventory_AddItem(3, 112);
    Inventory_AddItem(3, 130);
    Inventory_AddItem(3, 142);
    Inventory_AddItem(3, 171);
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(3);
    Owner_RecalculateStats(2);
}

void DebugMenu_SelectItem(void)
{
    struct RenderInput *window;
    struct RenderInput *details;
    s32 item;
    s32 redraw;
    s32 index;
    s32 count;

    count = 270;
    Engine_AudioPlayCue(112);
    window = Engine_DebugCreateWindow(0, 0, 30, 7, 2);
    details = Engine_DebugCreateWindow(0, 8, 13, 10, 2);
    item = 1;
    redraw = 1;
    Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
            (volatile u32 *)0x040000d4);
    Engine_TaskWait(1);
    for (;;) {
        if (redraw) {
            redraw = 0;
            item = (item + 270) % count;
            Engine_DebugRedrawWindow(window);
            Engine_DebugClearWindow(window);
            UiText_DrawStringInWindow(gDebugItemPrompt, window, 0, 0);
            UiText_DrawNumberAtOffset(item, 0, window, 80, 0);
            if (PartyInventory_HasSpace()) {
                UiText_DrawStringInWindow(gDebugItemCapacityLabel, window, 0, 32);
                index = item & 0x1ff;
                Engine_DebugGetItem(index);
                UiText_DrawCharacterAtOffset(index + (s32)MsgItemName, window, 120, 0);
                UiText_DrawResource(index + (s32)MsgItemPlainName, window, 0, 16);
                Engine_DebugRedrawWindow(details);
                Engine_DebugDrawItemDetails(details, item);
            } else {
                UiText_DrawStringInWindow(gDebugItemFullLabel, window, 0, 32);
            }
        }
        if (gKeyState & 1) {
            if (PartyInventory_Add(item) == -1) {
                Engine_AudioPlayCue(113);
                goto done;
            }
            Engine_AudioPlayCue(175);
        }
        if (gKeyState & 2) {
            Engine_AudioPlayCue(113);
            goto done;
        }
        if (gKeysRepeat & 64) {
            item--;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 128) {
            item++;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 16) {
            item += 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 32) {
            item -= 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 256) {
            item += 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 512) {
            item -= 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        Engine_TaskWait(1);
    }
done:
    Engine_DebugRedrawWindow(window);
    Engine_TaskWait(1);
    Engine_DebugFinalizeWindow(window, 1);
    Engine_DebugFinalizeWindow(details, 1);
}

void SceneState_RunCall1c00(void)
{

    DebugMenu_SelectItem();
}

void FieldScene_AssignCodeSetBToSlots(void)
{
    UiText_ShowPositionedMessageAndWait((s32)MsgDebugGotTreasure, 1);
    Inventory_AddItem(ROBIN, 0xb8);
    Inventory_AddItem(ROBIN, 0xcc);
    Inventory_AddItem(ROBIN, 0xdc);
    Inventory_AddItem(ROBIN, 0xdd);
    Inventory_AddItem(ROBIN, 0xde);
    Inventory_AddItem(ROBIN, 0xdf);
    Inventory_AddItem(ROBIN, 0xe0);
    Inventory_AddItem(JERARD, 0xe2);
    Inventory_AddItem(JERARD, 0xe3);
    Inventory_AddItem(JERARD, 0xe6);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe4);
    Inventory_AddItem(JERARD, 0xe5);
    Inventory_AddItem(JERARD, 0xe5);
    Inventory_AddItem(JERARD, 0xe5);
    Inventory_AddItem(JERARD, 0xe5);
    Inventory_AddItem(JERARD, 0xe5);
    Inventory_AddItem(JERARD, 0xe5);
    Inventory_AddItem(JERARD, 0xe5);
    Inventory_AddItem(JERARD, 0xe5);
    Inventory_AddItem(JERARD, 0xe8);
    Inventory_AddItem(JERARD, 0xe7);
    Inventory_AddItem(JERARD, 0xed);
    Inventory_AddItem(IWAN, 0xf2);
    Inventory_AddItem(IWAN, 0x102);
    Inventory_AddItem(IWAN, 0x10b);
    Inventory_AddItem(IWAN, 0x109);
    Inventory_AddItem(IWAN, 0xfc);
    Inventory_AddItem(MEARI, 0xbd);
    Inventory_AddItem(MEARI, 0xc8);
    Inventory_AddItem(MEARI, 0xc9);
    Inventory_AddItem(MEARI, 0xca);
    Inventory_AddItem(MEARI, 0xcb);
    Inventory_AddItem(MEARI, 0xcc);
    Inventory_AddItem(MEARI, 0xcf);
    Owner_RecalculateStats(ROBIN);
    Owner_RecalculateStats(JERARD);
    Owner_RecalculateStats(MEARI);
    Owner_RecalculateStats(IWAN);
}

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
    /* FAKEMATCH: the first three calls only match through a cast pointer */
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(ROBIN, 50);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(JERARD, 30);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(MEARI, 30);
    Party_AdvanceOwnerCountToTarget(IWAN, 30);
    Owner_RecalculateStats(ROBIN);
    Owner_RecalculateStats(JERARD);
    Owner_RecalculateStats(MEARI);
    Owner_RecalculateStats(IWAN);
    return 0;
}

s32 SceneState_GetFarResult2384(void)
{
    return DebugMenu_BrowseIcons();
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
s32 SceneState_GetFarResult2418(void)
{
    return Shop_ConfirmAct();
}
#endif
