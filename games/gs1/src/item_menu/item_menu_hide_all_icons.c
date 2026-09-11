#include "scene.h"
#include "item_menu.h"
#include "global_cells.h"
#include "far_runtime.h"

/* item_menu/icon/hide_all.c */
void ItemMenu_HideAllIcons(void)
{
    s32 hidden_state = 13;
    struct ItemMenuIcon **icons = gIw->entry_icons;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct ItemMenuIcon *icon = *icons++;
        if (icon != 0) {
            icon->state = hidden_state;
        }
    }
}

/* item_menu/icon/hide_page.c */
s32 Modulo(s32, s32);

void ItemMenu_HidePageIcons(void)
{
    struct ItemMenuState *menu = gIw;
    s32 slot = 0;
    s32 hidden_state = 13;
    struct ItemMenuIcon **icon_slot = menu->entry_icons;

    do {
        struct ItemMenuIcon *icon = *icon_slot++;

        if (icon != 0 && Modulo(slot, 5) == 0) {
            icon->state = hidden_state;
        }
        slot++;
    } while (slot <= 31);
}

/* item_menu/close.c */
void WaitFrames(s32);

void Menu_ReleaseEntryObjects(void);

void ItemMenu_Close(void)
{
    u8 *menu;
    s8 *cursor;

    menu = *(u8 **)ADDR_03001F2C;
    Menu_ReleaseEntryObjects();
    ItemMenu_HideAllIcons();
    WaitFrames(1);
    cursor = *(s8 **)(menu + 0x17C);
    cursor[5] = 0xD;
    Sys_Apply(menu + 0x10, 1);
    Sys_Apply(menu + 0x20, 1);
    Sys_Apply(menu + 0x10C, 1);
    Sys_Apply(menu + 0x24, 1);
    Sys_Apply(menu + 0x28, 1);
    Sys_Apply(menu + 0x2C, 1);
    Sys_Apply(menu + 0x30, 1);
    Sys_Apply(menu + 0x34, 1);
    Sys_Apply(menu + 0x38, 1);
    Sys_Apply(menu + 0x3C, 1);
    Sys_Apply(menu + 0x40, 1);
}

/* item_menu/prep_owner.c */
s32 Runtime_GetObject(s32);

void WaitFrames(s32);

s32 ItemMenu_PrepOwner(s32 party_slot)
{
    register void *menu = gIw;
    register s32 offset = party_slot + 28;
    s32 result = 0;
    register s32 owner_index;
    u8 *owner_slots;

    owner_index = *(s8 *)(menu + offset);
    Sys_Do(*(s32 *)(menu + 44));
    owner_slots = menu + 2;
    owner_slots[offset] = *(u8 *)(menu + 0x219);
    if (owner_index == -1) {
        *(u8 *)(menu + offset) = result;
        offset = 0;
    } else {
        offset = owner_index * 2;
        Sys_Apply(owner_index * 24 - 10, 16);
    }

    {
        s32 off = offset + 0x208;
        result = Runtime_GetObject(*(u16 *)(menu + off));
    }
    offset = (s32)(menu + 0x1C8);
    *(s8 *)(menu + 0x218) =
        (s8)ItemMenu_Collect((void *)result, (u16 *)offset, 0);
    result = Sys_Apply2(menu + 0x208, (void *)offset);
    {
        s32 cursor_offset = party_slot * 4 + 20;
        UiIcon_PrepareObject(*(s32 *)(menu + cursor_offset));
    }
    WaitFrames(1);
    return result;
}

/* item_menu/open_category.c */
void ItemMenu_OpenCategory(s32 owner_id)
{
    struct ItemMenuState *menu;

    menu = gIw;
    Sys_Run();
    UiWindow_Commit(menu->item_window);
    ItemMenu_DrawCategory(menu->item_window, owner_id, 0);
}
