#include "scene.h"
#include "abi/item_menu/open_detail.h"
#include "item_menu.h"

struct ItemMenuDetailsState {
    u8 unknown_000[0x30];
    s32 details_window;
    u8 unknown_034[0x144];
    u16 item_ids[0x100];
};

s32 ItemMenu_OpenDetail(s32 item_index)
{
    struct ItemMenuDetailsState *menu =
        (struct ItemMenuDetailsState *)gIw;

    Sys_SetRect(&menu->details_window, 0, 0, 13, 10, 2);
    Sys_Run();

    if (menu->item_ids[item_index] != 0)
        ItemMenu_DrawItemDetails(
            menu->details_window, menu->item_ids[item_index]);

    return 1;
}
