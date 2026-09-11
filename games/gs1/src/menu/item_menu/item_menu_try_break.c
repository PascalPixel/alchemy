#include "scene.h"
#include "fixed_math.h"
#include "item_menu.h"
#include "global_cells.h"
#include "inventory.h"
#include "item.h"
#include "sound_ids.h"

/* item_menu/try_break.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

u32 Random16();
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16

s32 Audio_PlayCue(s32);

void ItemMenu_TryBreak(void)
{
    void *menu;

    menu = *(void **)ADDR_03001F2C;
    if ((FIELD(Item_Get(0x1FF & FIELD(menu, u16 *, 0x178)), u8 *, 0xC) == 2) && (Rand() < 0x2000U)) {
        Inventory_BreakFar(
            FIELD(menu, u8 *, 0x21A),
            FIELD(menu, u16 *, 0x174));
        Audio_PlayCue(SOUND_ITEM_BREAK);
        Sys_Place(0xB86, 0, -1);
    }
}

/* item_menu/open_detail.c */
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
