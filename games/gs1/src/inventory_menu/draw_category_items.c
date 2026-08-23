#include "inventory_menu.h"
#include "owner_state.h"

extern u8 Value_00000b24;

extern void Func_080a9cbc(void);
extern void UiText_DrawAt(void *, s32, s32, s32);
extern s32 Func_080a9aec(s32, void *);
extern void WaitFrames(s32);
extern s32 Func_080a9c18(void *);

void InventoryMenu_DrawCategoryItems(s32 window, s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu = Data_03001f2c;
    u8 *items;

    OwnerState_GetFar(owner_id);
    Func_080a9cbc();
    InventoryMenu_HideAllItemIcons();
    UiText_DrawAt(&Value_00000b24, window, 0, 0);
    UiText_DrawAt(&Value_00000b24 + 1, window, 0, 32);
    UiText_DrawAt(&Value_00000b24 + 2, window, 0, 16);
    UiText_DrawAt(&Value_00000b24 + 3, window, 0, 48);
    items = (u8 *)menu->items;
    Func_080a9aec(window, items);
    if (mode == 0) {
        WaitFrames(1);
        InventoryMenu_DrawItemIcons((u16 *)items, 1);
        Func_080a9c18(items);
    }
}
