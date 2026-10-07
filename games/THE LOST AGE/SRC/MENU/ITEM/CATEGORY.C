#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "text/MSG_IDS.H"

void ItemMenu_HideAllIcons(void);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void ItemMenu_DrawIcons(u16 *items, s32 mode);

struct CategoryMenuState {
    u8 unknown_000[452];
    u16 items[15];
};

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct CategoryMenuState *menu = Ram_HeapSlots->menu_runtime;
    u8 *items;

    ItemMenu_PosCategory();
    ItemMenu_HideAllIcons();
    UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels, window, 0, 0);
    UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 1, window, 0, 32);
    UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 2, window, 0, 16);
    UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 3, window, 0, 48);
    items = (u8 *)menu->items;
    ItemMenu_DrawEquippedItemNames(window, items);
    if (mode == 0) {
        WaitFrames(1);
        ItemMenu_DrawIcons((u16 *)items, 1);
        ItemMenu_ArrangeCategoryItemIcons(items);
    }
}
