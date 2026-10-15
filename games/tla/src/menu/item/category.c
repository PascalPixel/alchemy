#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

void ItemMenu_HideAllIcons(void);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void ItemMenu_DrawIcons(u16 *items, s32 mode);

struct CategoryMenuState {
    u8 unk000[0x1c4];
    u16 items[15];
};

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct CategoryMenuState *menu = Ram_HeapSlots->menuRuntime;
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
        ItemMenu_ArrangeCategoryItemIcons((u16 *)items);
    }
}
