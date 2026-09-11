#include "item_menu.h"

extern u8 Value_00000182;
#define ItemMenu_ItemNameMessages Value_00000182

extern void Func_080a17c4(void *icon);
extern s32 Runtime_GetObject(s32);
extern void Func_08015090(s32, void *, s32, s32);
extern void UiText_DrawAt(s32, void *, s32, s32);

void ItemMenu_DrawItemHead(void)
{
    struct ItemMenuState *menu = Data_03001f2c;

    ItemMenu_DrawIcon(
        2, menu->selected_item, menu->selected_item_icon->render_target, 0);
    menu->selected_item_icon->state = 1;
    menu->selected_item_icon->x = 112;
    menu->selected_item_icon->y = 8;
    Func_080a17c4(menu->selected_item_icon);
    Func_08015090(
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
