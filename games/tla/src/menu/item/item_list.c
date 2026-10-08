#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ItemListMenu {
    u8 unknown_000[0xf0];
    struct RenderOutput *message_window;
};

void RenderOutput_RedrawSavedRectFar(struct RenderOutput *window);
s32 UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

s32 ItemMenu_IsSpecial(s32 item_id)
{
    s32 result;
    s32 first;

    if (item_id > 0xC4) {
        goto L0;
    }
    first = 0xC1;
    if (item_id < first) {
        goto L0;
    }
    result = 1;
    return result;
L0:
    result = 0;
    return result;
}

void ItemMenu_DrawMsg(s32 unused, s32 message)
{
    struct ItemListMenu *menu;

    menu = Ram_HeapSlots->menu_runtime;
    RenderOutput_RedrawSavedRectFar(menu->message_window);
    UiText_DrawCharacterAtOffsetFar(message, (s32)menu->message_window, 0, 0);
}
