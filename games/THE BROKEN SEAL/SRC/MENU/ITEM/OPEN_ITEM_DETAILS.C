#include "INVENTORY_MENU.H"

struct InventoryMenuDetailsState {
    u8 unknown_000[0x30];
    s32 details_window;
    u8 unknown_034[0x144];
    u16 item_ids[0x100];
};

s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void Palette_CopyObjectBankToBackground14(void);
void ItemMenu_DrawItemDetails(s32, s32);

s32 ItemMenu_OpenDetail(s32 item_index)
{
    struct InventoryMenuDetailsState *menu =
        (struct InventoryMenuDetailsState *)gMenuWork;

    UiWindow_UpdateOrCreate(&menu->details_window, 0, 0, 13, 10, 2);
    Palette_CopyObjectBankToBackground14();

    if (menu->item_ids[item_index] != 0)
        ItemMenu_DrawItemDetails(
            menu->details_window, menu->item_ids[item_index]);

    return 1;
}
