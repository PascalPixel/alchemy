#include "MENU_TEST.H"

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

    Call2(UiText_ShowPositionedMessageAndWait, 0xc1a, 1);
    p = (s32 *)MenuTest_SlotOffsets;
    Party_AdvanceOwnerCountToTarget(0, *p);
    Value2(Party_AdvanceOwnerCountToTarget, 1, *p);
    Value2(Party_AdvanceOwnerCountToTarget, 3, *p);
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

    Call2(UiText_ShowPositionedMessageAndWait, 0xc1e, 1);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 187);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 180);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 181);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 182);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 0, 183);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 186);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 1, 187);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 188);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 189);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 2, 236);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 191);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 192);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 193);
    Value2(Inventory_AddItem, 3, 194);
    Value2(Inventory_AddItem, 3, 194);
    Value2(Inventory_AddItem, 3, 194);
    Value2(Inventory_AddItem, 3, 194);
    Value2(Inventory_AddItem, 3, 194);
    Value2(Inventory_AddItem, 3, 194);
    Value2(Inventory_AddItem, 3, 194);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 195);
    Value2(Inventory_AddItem, 3, 196);
    Value2(Inventory_AddItem, 3, 196);
    Value2(Inventory_AddItem, 3, 196);
    Value2(Inventory_AddItem, 3, 196);
    Value2(Inventory_AddItem, 3, 196);
    Value2(Inventory_AddItem, 3, 196);
    Value2(Inventory_AddItem, 3, 196);
    Inventory_AddItem(3, 196);
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(3);
    Owner_RecalculateStats(2);
}

void CommandTable_ConfigureCommandGroups(void)
{
    u8 buf[256];
    UiText_ShowPositionedMessageAndWait(0xc1d, 1);
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

    Call2(UiText_ShowPositionedMessageAndWait, 0xc1b, 1);
    Value2(Owner_AdjustFirstValue, 0, -100);
    Value2(Owner_AdjustFirstValue, 1, -100);
    Value2(Owner_AdjustFirstValue, 2, -33);
    Value2(Owner_AdjustFirstValue, 3, -100);
    Value2(Owner_AdjustSecondValue, 0, -50);
    Value2(Owner_AdjustSecondValue, 1, -40);
    Value2(Owner_AdjustSecondValue, 2, -35);
    Call2(Owner_AdjustSecondValue, 3, -20);
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
    UiText_ShowPositionedMessageAndWait(0xc1f, 1);
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
