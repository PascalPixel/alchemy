#include "scene.h"
#include "item_menu.h"
#include "m7_interfaces.h"
#include "global_cells.h"

/* item_menu/draw/stat_delta.c */
extern u8 ItemMenu_IncreaseGlyph;
extern u8 ItemMenu_DecreaseGlyph;

void UiNumber_DrawAt(s32, s32, s32, s32, s32);

void ItemMenu_DrawStat(
    s32 delta, s32 unused, s32 window, s32 x, s32 y)
{
    s32 digits;
    s32 magnitude;

    UiNumber_DrawAt(delta, 3, window, x, y);
    digits = 1;
    magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude > 9) {
        digits = 2;
    }
    magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude > 99) {
        digits = 3;
    }
    if (delta > 0) {
        Sys_SetMode(
            &ItemMenu_IncreaseGlyph, window, x - digits * 8 + 16, y);
    } else {
        Sys_SetMode(
            &ItemMenu_DecreaseGlyph, window, x - digits * 8 + 16, y);
    }
}

/* item_menu/win/item_5.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void ItemMenu_SetItemWin5(void)
{
    void *menu = *(void **)ADDR_03001F2C;
    struct WindowBounds *window =
        FIELD(menu, struct WindowBounds **, 0x20);

    WindowBounds_Set(window, 0xD, 5, 0x11, 0xA);
}

/* item_menu/win/item_3.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void ItemMenu_SetItemWin3(void)
{
    void *menu = *(void **)ADDR_03001F2C;
    struct WindowBounds *window =
        FIELD(menu, struct WindowBounds **, 0x20);

    WindowBounds_Set(window, 0xD, 3, 0x11, 0xA);
}

/* item_menu/win/msg_5.c */
void ItemMenu_SetMsgWin5(void)
{
    WindowBounds_Set(
        (struct WindowBounds *)gIw->message_window,
        13, 0, 17, 5);
}

/* item_menu/win/msg_6.c */
void ItemMenu_SetMsgWin6(void)
{
    WindowBounds_Set(
        (struct WindowBounds *)gIw->message_window,
        13, 0, 17, 6);
}

/* item_menu/win/msg_7.c */
void ItemMenu_SetMsgWin7(void)
{
  int state_cell;
  state_cell = ADDR_03001F2C;
  WindowBounds_Set(*((struct WindowBounds **) (((u8 *)(*((void **)state_cell))) + 0x10C)), 0xD, 0, 0x11, 7);
}

/* item_menu/win/msg_3.c */
void ItemMenu_SetMsgWin3(void)
{
    WindowBounds_Set(
        (struct WindowBounds *)gIw->message_window,
        13, 0, 17, 3);
}

/* item_menu/draw/item_head.c */
extern u8 Value_00000182;
#define ItemMenu_ItemNameMessages Value_00000182

extern s32 Runtime_GetObject(s32);

extern void UiText_DrawAt(s32, void *, s32, s32);

void ItemMenu_DrawItemHead(void)
{
    struct ItemMenuState *menu = gIw;

    ItemMenu_DrawIcon(
        2, menu->selected_item, menu->selected_item_icon->render_target, 0);
    menu->selected_item_icon->state = 1;
    menu->selected_item_icon->x = 112;
    menu->selected_item_icon->y = 8;
    Sys_Do(menu->selected_item_icon);
    Sys_SetMode(
        Runtime_GetObject(menu->item_owner),
        (void *)menu->message_window,
        16,
        0);
    UiText_DrawAt(
        (menu->selected_item & 0x1FF) +
            (s32)&ItemMenu_ItemNameMessages,
        (void *)menu->message_window,
        16,
        8);
}
