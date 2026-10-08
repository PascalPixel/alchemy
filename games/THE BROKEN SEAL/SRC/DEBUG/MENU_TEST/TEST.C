#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "MENU_TEST.H"
#include "CALL.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "INVENTORY.H"
#include "DMA.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "ITEM.H"
#include "ITEMGIFT.H"
#include "TBS_EDITION.H"
#include "text/MSG_IDS.H"

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)


void Event_SetMessageId(s16 message);


void Engine_AudioPlayCue(s32 cue);
struct RenderInput *Engine_DebugCreateWindow(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawStringInWindow(u8 *text, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 format, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawResource(s32 text, struct RenderInput *window, s32 x, s32 y);


u8 *MenuTest_GetEntrances(void)
{
    return MenuTest_Entrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *MenuTest_GetExits(void)
{
    return MenuTest_Exits;
}

u8 *MenuTest_GetPlacements(void)
{
    return MenuTest_Placements;
}

#if EDITION_INTERNATIONAL
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
    volatile s32 *key;
    s32 token;
    s32 i;

    gGameState.message_speed = 2;
    token = UiWindow_CreateWithSideObject(125, 0, 0, 0);
    for (i = 0; i < cnt; i++) {
        key = &gKeysHeld;
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
    CommandTable_RunDirectionalInput((s32)MsgWeaponShopWelcome, MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockCc6(void)
{
    CommandTable_RunDirectionalInput((s32)MsgArmorShopWelcome, MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockCf1(void)
{
    CommandTable_RunDirectionalInput((s32)MsgItemShopWelcome, MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockD21(void)
{
    CommandTable_RunDirectionalInput((s32)MsgSanctumWelcome, MsgWarriorShopWelcome - MsgSanctumWelcome);
}

void SceneState_ApplyBlockD4c(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorShopWelcome, MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockD77(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorArmorShopWelcome, MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void SceneState_ApplyBlockDa2(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorItemShopWelcome, MsgArmorShopWelcome - MsgWeaponShopWelcome);
}
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
void DebugMenu_RunMessageTest(void)
{
    Event_SetMessageId(MSGNO(MsgBattleAdditionalPsynergyHelp));
    Event_SetMessageId(MSGNO(MsgHaidiaComeOnHurry));
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
    gEffectWork->child_mode = 1;
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
void DebugMenu_GiveItemToParty(void)
{
    PartyInventory_GiveItem(181, 0);
}
#endif

s32 MenuTest_GetEvents(void)
{
    return (s32)MenuTest_Events;
}

void MenuTest_RaisePartyLevelTarget(void)
{
    s32 *p;

    UiText_ShowPositionedMessageAndWait(MsgDebugWentLevel, 1);
    p = (s32 *)MenuTest_SlotOffsets;
    Party_AdvanceOwnerCountToTarget(0, *p);
    Party_AdvanceOwnerCountToTarget(1, *p);
    Party_AdvanceOwnerCountToTarget(3, *p);
    Party_AdvanceOwnerCountToTarget(2, *p);
    *p += 10;
    UnitCalc(0);
    UnitCalc(1);
    UnitCalc(3);
    UnitCalc(2);
}

void FieldScene_GrantItemListToSlots(void)
{
    u32 tmp;
    s32 slot;

    UiText_ShowPositionedMessageAndWait(MsgDebugGotItem, 1);
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
    UnitCalc(0);
    UnitCalc(1);
    UnitCalc(3);
    UnitCalc(2);
}

void CommandTable_ConfigureCommandGroups(void)
{
    u8 buf[256];
#if !EDITION_INTERNATIONAL
    s32 i;
    s32 j;

    for (i = 0; i < 16; i++) {
        for (j = 255; j >= 0; j--) {
            buf[j] = 0;
        }
        /* FAKEMATCH: The loop-index constraint avoids GCC's extra r6 address copy/save. */
        asm("" : "+r"(i));
        for (j = 0; j < i + 32; j++) {
            buf[j] = MenuTest_BenchmarkCharacter + 1;
        }
        MenuTest_BenchmarkCharacter++;
        if (MenuTest_BenchmarkCharacter > 96) {
            MenuTest_BenchmarkCharacter = 65;
        }
    }
#endif
    UiText_ShowPositionedMessageAndWait(MsgDebugGotDjinni, 1);
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
    UnitCalc(0);
    UnitCalc(1);
    UnitCalc(3);
    UnitCalc(2);
}

void FieldScene_ApplySlotOffsetsAndFlags(void)
{
    u32 i;
    u8 *record;

    UiText_ShowPositionedMessageAndWait(MsgDebugBodyTornApart, 1);
    Value2(Owner_AdjustFirstValue, 0, -100);
    Value2(Owner_AdjustFirstValue, 1, -100);
    Owner_AdjustFirstValue(2, -33);
    Owner_AdjustFirstValue(3, -100);
    UnitEpAdd(0, -50);
    UnitEpAdd(1, -40);
    UnitEpAdd(2, -35);
    UnitEpAdd(3, -20);
    record = UnitGet(0);
    record[0x131] = 1;
    record += 0x140;
    *record = 1;
    record = UnitGet(1);
    record[0x130] = 1;
    record[0x131] = 2;
    UnitCalc(0);
    UnitCalc(1);
    UnitCalc(3);
    UnitCalc(2);
}

void FieldScene_AssignCodeSetAToSlots(void)
{
    UiText_ShowPositionedMessageAndWait(MsgDebugGotSturdyEquipment, 1);
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
    UnitCalc(0);
    UnitCalc(1);
    UnitCalc(3);
    UnitCalc(2);
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
            REG_DMA3);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
            REG_DMA3);
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
        if (gKeyState & KEY_A) {
            if (PartyInventory_Add(item) == -1) {
                Engine_AudioPlayCue(113);
                goto done;
            }
            Engine_AudioPlayCue(175);
        }
        if (gKeyState & KEY_B) {
            Engine_AudioPlayCue(113);
            goto done;
        }
        if (gKeysRepeat & KEY_UP) {
            item--;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & KEY_DOWN) {
            item++;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & KEY_RIGHT) {
            item += 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & KEY_LEFT) {
            item -= 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & KEY_R) {
            item += 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & KEY_L) {
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
    UiText_ShowPositionedMessageAndWait(MsgDebugGotTreasure, 1);
    Inventory_AddItem(ROBIN, 0xb8);
    Inventory_AddItem(ROBIN, 0xcc);
    Inventory_AddItem(ROBIN, 0xdc);
    Inventory_AddItem(ROBIN, 0xdd);
    Inventory_AddItem(ROBIN, 0xde);
    Inventory_AddItem(ROBIN, 0xdf);
    Inventory_AddItem(ROBIN, 0xe0);
    Inventory_AddItem(GERALD, 0xe2);
    Inventory_AddItem(GERALD, 0xe3);
    Inventory_AddItem(GERALD, 0xe6);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe4);
    Inventory_AddItem(GERALD, 0xe5);
    Inventory_AddItem(GERALD, 0xe5);
    Inventory_AddItem(GERALD, 0xe5);
    Inventory_AddItem(GERALD, 0xe5);
    Inventory_AddItem(GERALD, 0xe5);
    Inventory_AddItem(GERALD, 0xe5);
    Inventory_AddItem(GERALD, 0xe5);
    Inventory_AddItem(GERALD, 0xe5);
    Inventory_AddItem(GERALD, 0xe8);
    Inventory_AddItem(GERALD, 0xe7);
    Inventory_AddItem(GERALD, 0xed);
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
    UnitCalc(ROBIN);
    UnitCalc(GERALD);
    UnitCalc(MEARI);
    UnitCalc(IWAN);
}

s32 CommandTable_ConfigureCommandList(void)
{
    Party_RemoveActiveOwner(5);
    Party_AddActiveOwner(GERALD);
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
    /* FAKEMATCH: the typed void calls put the first three owner arguments before the target count; ordinary calls reverse their order. */
#if EDITION_INTERNATIONAL
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(ROBIN, 50);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(GERALD, 30);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(MEARI, 30);
#else
    gGameState.coins = 777777;
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(ROBIN, 30);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(GERALD, 30);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(MEARI, 30);
#endif
    Party_AdvanceOwnerCountToTarget(IWAN, 30);
    UnitCalc(ROBIN);
    UnitCalc(GERALD);
    UnitCalc(MEARI);
    UnitCalc(IWAN);
    return 0;
}

s32 SceneState_GetFarResult2384(void)
{
    return DebugMenu_BrowseIcons();
}

#if !EDITION_INTERNATIONAL
s32 DebugMenu_SelectCharacter(void)
{
    return Menu_OpenCharacterSelector();
}
#endif

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
s32 SceneState_GetFarResult2418(void)
{
    return Shop_ConfirmAct();
}
#endif
