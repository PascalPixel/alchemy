#include "scene.h"
#include "item_menu.h"
#include "owner_state.h"

extern u8 Value_00000b24;

extern void UiText_DrawAt(void *, s32, s32, s32);

extern void WaitFrames(s32);

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct ItemMenuState *menu = gIw;
    u8 *items;

    OwnerState_GetFar(owner_id);
    Sys_Run();
    ItemMenu_HideAllIcons();
    UiText_DrawAt(&Value_00000b24, window, 0, 0);
    UiText_DrawAt(&Value_00000b24 + 1, window, 0, 32);
    UiText_DrawAt(&Value_00000b24 + 2, window, 0, 16);
    UiText_DrawAt(&Value_00000b24 + 3, window, 0, 48);
    items = (u8 *)menu->items;
    Sys_Apply(window, items);
    if (mode == 0) {
        WaitFrames(1);
        ItemMenu_DrawIcons((u16 *)items, 1);
        Sys_Check(items);
    }
}
