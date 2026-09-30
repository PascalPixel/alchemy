#include "MENU_TEST.H"
#include "CALL.H"

extern u8 MsgSanctumWelcome[];
extern u8 MsgWarriorShopWelcome[];

extern u8 MsgWarriorArmorShopWelcome[];

extern u8 MsgWarriorItemShopWelcome[];

extern u8 MsgDebugBodyTornApart[];
extern u8 MsgDebugGotDjinni[];
extern u8 MsgDebugGotItem[];
extern u8 MsgDebugGotSturdyEquipment[];
extern u8 MsgDebugWentLevel[];

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
    s32 record;

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
    *(u8 *)(record + 0x131) = 1;
    record = record + 0x140;
    *(u8 *)record = 1;
    record = Owner_GetState(1);
    *(u8 *)((record + 0x130)) = 1;
    *(u8 *)(record + 0x131) = 2;
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
