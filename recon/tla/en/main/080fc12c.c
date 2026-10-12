/* Draft, differ 8 of 57: The Broken Seal's ItemMenu_DrawItemHead (the icon must be the shared
 * struct RenderOutput for the native's reloads of the icon pointer after each store). Four rows
 * are scheduled the other way round: the icon load before the item offset 0x16c is started (ldr
 * r3, [r5]; movs r2, #182), the 112 and the 8 before the icon reloads (movs r3, #112; ldr r2,
 * [r5]; strh), and the item load before 0x1ff is started (ldrh r3, [r2]; movs r0, #128). One-pass
 * blocks at each of the three places measured 8 to 17. MsgItemName is not named in the catalogs
 * yet. */
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct InventoryMenuState {
    u8 unknown_000[0xf0];
    s32 message_window;
    u8 unknown_0f4[0x78];
    u16 selected_items[2];
    u8 unknown_170[0xa6];
    u8 pane_owner[2];
    struct RenderOutput *selected_item_icon;
};

extern char MsgItemName[];

void *UnitGet(s32 owner);
void Func_08038288(s32 mode, s32 item, s32 slot, s32 flags);
void UiIcon_PrepareObject(struct RenderOutput *icon);
void UiText_DrawStringAtOffsetFar(void *string, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

// 選んだどうぐの絵と、持ち主・どうぐの名前を見出しに描く
void ItemMenu_DrawItemHead(void)
{
    struct InventoryMenuState *menu = Ram_HeapSlots->menu_runtime;

    Func_08038288(2, menu->selected_items[0], (u8)menu->selected_item_icon->index, 0);
    menu->selected_item_icon->active = 1;
    menu->selected_item_icon->x = 112;
    menu->selected_item_icon->y = 8;
    UiIcon_PrepareObject(menu->selected_item_icon);
    UiText_DrawStringAtOffsetFar(UnitGet(menu->pane_owner[0]), menu->message_window, 16, 0);
    UiText_DrawCharacterAtOffsetFar(
        (menu->selected_items[0] & 0x1ff) + (s32)MsgItemName, menu->message_window, 16, 8);
}
