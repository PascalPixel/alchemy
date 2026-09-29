#include "MENU_TEST.H"

void SceneState_RunCall1c00(void)
{

    DebugMenu_SelectItem();
}

void FieldScene_AssignCodeSetBToSlots(void)
{
    UiText_ShowPositionedMessageAndWait(0xc1c, 1);
    Inventory_AddItem(0, 0xb8);
    Inventory_AddItem(0, 0xcc);
    Inventory_AddItem(0, 0xdc);
    Inventory_AddItem(0, 0xdd);
    Inventory_AddItem(0, 0xde);
    Inventory_AddItem(0, 0xdf);
    Inventory_AddItem(0, 0xe0);
    Inventory_AddItem(1, 0xe2);
    Inventory_AddItem(1, 0xe3);
    Inventory_AddItem(1, 0xe6);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe4);
    Inventory_AddItem(1, 0xe5);
    Inventory_AddItem(1, 0xe5);
    Inventory_AddItem(1, 0xe5);
    Inventory_AddItem(1, 0xe5);
    Inventory_AddItem(1, 0xe5);
    Inventory_AddItem(1, 0xe5);
    Inventory_AddItem(1, 0xe5);
    Inventory_AddItem(1, 0xe5);
    Inventory_AddItem(1, 0xe8);
    Inventory_AddItem(1, 0xe7);
    Inventory_AddItem(1, 0xed);
    Inventory_AddItem(2, 0xf2);
    Inventory_AddItem(2, 0x102);
    Inventory_AddItem(2, 0x10b);
    Inventory_AddItem(2, 0x109);
    Inventory_AddItem(2, 0xfc);
    Inventory_AddItem(3, 0xbd);
    Inventory_AddItem(3, 0xc8);
    Inventory_AddItem(3, 0xc9);
    Inventory_AddItem(3, 0xca);
    Inventory_AddItem(3, 0xcb);
    Inventory_AddItem(3, 0xcc);
    Inventory_AddItem(3, 0xcf);
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(3);
    Owner_RecalculateStats(2);
}

s32 CommandTable_ConfigureCommandList(void)
{
    Party_RemoveActiveOwner(5);
    Party_AddActiveOwner(1);
    Party_AddActiveOwner(3);
    Party_AddActiveOwner(2);
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
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(0, 50);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(1, 30);
    ((void (*)(s32, s32))Party_AdvanceOwnerCountToTarget)(3, 30);
    Party_AdvanceOwnerCountToTarget(2, 30);
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(3);
    Owner_RecalculateStats(2);
    return 0;
}

s32 SceneState_GetFarResult2384(void)
{
    return DebugMenu_BrowseIcons();
}

s32 SceneState_GetFarResult2418(void)
{
    return Shop_ConfirmAct();
}
