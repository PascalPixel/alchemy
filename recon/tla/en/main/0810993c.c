#include "BATTLE_RUNTIME.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];
extern u8 Data_03001c94[];
extern u8 gKeysRepeat[];

s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_FindEquippedFar(s32, u8);
s32 Party_AdjustSixDigitCounterAFar(s32);
s32 Party_AdjustSixDigitCounterBFar(s16);
void UiMessage_ShowAndRestoreState(s32 message);
void Audio_PlayCue(s32);
extern char MsgHereYouGo;

s32 Shop_ConfirmEquip(s32 unit_id, s32 slot)
{
    struct ShopRuntime *menu = gMenuWork;
    struct BattleUnit *unit = (struct BattleUnit *)Owner_GetStateFar(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *info = Item_Get(item_id);
    s32 replaced;
    s32 menu_value;

    if (unit->inventory[slot] & 0x200)
        return 0;

    if (Item_CanOwnerEquip(unit_id, item_id) == 0)
        return 0;

    replaced = Inventory_FindEquippedFar(unit_id, info->type);
    if (replaced != -1) {
        struct ItemDefinition *old_info = Item_Get(unit->inventory[replaced]);

        if (old_info->flags & 2)
            return 0;
    }

    UiWork_PushValueSlotFar(unit_id, 1);
    UiMessage_ShowAndWait((s32)&MsgEquipNowPrompt);
    if (UiMessage_ShowChoice(0) != 0)
        return 0;

    Inventory_EquipFar(unit_id, slot);
    menu_value = menu->item_window;
    if (menu_value != 0)
        Shop_DrawUnitGrid(menu_value, unit_id);

    if (info->flags & 1) {
        Audio_PlayCue(103);
        UiWork_FinalizePendingCoreFar();
        UiText_OpenMessageWindowFar((s32)MsgBecameCursed, 8, 4, 2);
        while (UiWork_IsCompleteFar() == 0) {
            WaitFrames(1);
        }
    }

    UiMessage_ShowAndRestoreState((s32)&MsgLookBolder);
    return 1;
}
