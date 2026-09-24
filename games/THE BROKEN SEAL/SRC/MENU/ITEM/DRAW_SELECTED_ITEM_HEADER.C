#include "INVENTORY_MENU.H"

extern u8 MsgItemName;
#define InventoryMenu_ItemNameMessages MsgItemName

extern void UiIcon_PrepareObject(void *icon);
extern s32 Owner_GetStateFar(s32);
extern void UiText_DrawStringAtOffsetFar(s32, void *, s32, s32);
extern void UiText_DrawCharacterAtOffsetFar(s32, void *, s32, s32);

void ItemMenu_DrawItemHead(void)
{
    struct InventoryMenuState *menu = gMenuWork;

    InventoryMenu_DrawItemIcon(
        2, menu->selected_item, menu->selected_item_icon->render_target, 0);
    menu->selected_item_icon->state = 1;
    menu->selected_item_icon->x = 112;
    menu->selected_item_icon->y = 8;
    UiIcon_PrepareObject(menu->selected_item_icon);
    UiText_DrawStringAtOffsetFar(
        Owner_GetStateFar(menu->item_owner),
        (void *)menu->message_window,
        16,
        0);
    UiText_DrawCharacterAtOffsetFar(
        (menu->selected_item & 0x1FF) +
            (s32)&InventoryMenu_ItemNameMessages,
        (void *)menu->message_window,
        16,
        8);
}
