#include "INVENTORY_MENU.H"

s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void Palette_CopyObjectBankToBackground14(void);
void ItemMenu_DrawItemDetails(s32, s32);

s32 ItemMenu_OpenDetail(s32 item_index)
{
    struct InventoryMenuState *menu = gMenuWork;

    UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 0, 13, 10, 2);
    Palette_CopyObjectBankToBackground14();

    if (menu->selected_items[item_index] != 0)
        ItemMenu_DrawItemDetails(
            (s32)menu->equip_window, menu->selected_items[item_index]);

    return 1;
}
