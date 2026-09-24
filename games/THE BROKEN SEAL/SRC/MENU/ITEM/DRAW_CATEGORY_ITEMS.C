#include "INVENTORY_MENU.H"
#include "OWNER_STATE.H"
#include "SYSTEM.H"

extern u8 Value_00000b24;

extern void ItemMenu_PosCategory(void);
extern void UiText_DrawAt(void *, s32, s32, s32);
extern s32 ItemMenu_DrawEquippedItemNames(s32, void *);
extern s32 ItemMenu_ArrangeCategoryItemIcons(void *);

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu = gMenuWork;
    u8 *items;

    OwnerState_GetFar(owner_id);
    ItemMenu_PosCategory();
    InventoryMenu_HideAllItemIcons();
    UiText_DrawAt(&Value_00000b24, window, 0, 0);
    UiText_DrawAt(&Value_00000b24 + 1, window, 0, 32);
    UiText_DrawAt(&Value_00000b24 + 2, window, 0, 16);
    UiText_DrawAt(&Value_00000b24 + 3, window, 0, 48);
    items = (u8 *)menu->items;
    ItemMenu_DrawEquippedItemNames(window, items);
    if (mode == 0) {
        WaitFrames(1);
        InventoryMenu_DrawItemIcons((u16 *)items, 1);
        ItemMenu_ArrangeCategoryItemIcons(items);
    }
}
