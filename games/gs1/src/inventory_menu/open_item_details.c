#include "inventory_menu.h"

struct InventoryMenuDetailsState {
    u8 unknown_000[0x30];
    s32 details_window;
    u8 unknown_034[0x144];
    u16 item_ids[0x100];
};

s32 Func_080a10d0(s32 *, s32, s32, s32, s32, s32);
void Func_080a22f4(void);
void Func_080a4924(s32, u16);
#define InventoryMenu_DrawItemDetails Func_080a4924

s32 InventoryMenu_OpenItemDetails(s32 item_index)
{
    struct InventoryMenuDetailsState *menu =
        (struct InventoryMenuDetailsState *)Data_03001f2c;

    Func_080a10d0(&menu->details_window, 0, 0, 13, 10, 2);
    Func_080a22f4();

    if (menu->item_ids[item_index] != 0)
        InventoryMenu_DrawItemDetails(
            menu->details_window, menu->item_ids[item_index]);

    return 1;
}
