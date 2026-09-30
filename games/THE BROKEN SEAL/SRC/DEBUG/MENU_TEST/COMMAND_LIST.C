#include "MENU_TEST.H"
extern u8 MsgDebugGotTreasure[];

/* Party members, in party order. */
enum { ROBIN, JERARD, IWAN, MEARI };

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

s32 SceneState_GetFarResult2418(void)
{
    return Shop_ConfirmAct();
}
