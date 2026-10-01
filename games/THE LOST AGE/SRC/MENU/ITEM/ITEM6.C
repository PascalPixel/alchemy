#include "TYPES.H"
#include "RAM_BUFFER.H"

extern u8 MsgEquipSlotLabels;
void ItemMenu_PosCategory(void);
void ItemMenu_HideAllIcons(void);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void ItemMenu_DrawEquippedItemNames(s32 window, u8 *items);
void ItemMenu_DrawIcons(u16 *items, s32 mode);
void ItemMenu_ArrangeCategoryItemIcons(u8 *items);
void WaitFrames(s32);

/* ⚓️ keeps the owner's items 0x1c4 into the menu work and does not look the
   owner up first, as ☀️ does. */
struct CategoryMenuState {
    u8 unknown_000[0x1c4];
    u16 items[15];
};

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct CategoryMenuState *menu = Ram_HeapSlots->menu_runtime;
    u8 *items;

    ItemMenu_PosCategory();
    ItemMenu_HideAllIcons();
    UiText_DrawCharacterAtOffsetFar(&MsgEquipSlotLabels, window, 0, 0);
    UiText_DrawCharacterAtOffsetFar(&MsgEquipSlotLabels + 1, window, 0, 32);
    UiText_DrawCharacterAtOffsetFar(&MsgEquipSlotLabels + 2, window, 0, 16);
    UiText_DrawCharacterAtOffsetFar(&MsgEquipSlotLabels + 3, window, 0, 48);
    items = (u8 *)menu->items;
    ItemMenu_DrawEquippedItemNames(window, items);
    if (mode == 0) {
        WaitFrames(1);
        ItemMenu_DrawIcons((u16 *)items, 1);
        ItemMenu_ArrangeCategoryItemIcons(items);
    }
}
