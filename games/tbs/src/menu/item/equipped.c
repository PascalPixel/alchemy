#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "inventory_menu.h"
#include "owner_state.h"
#include "system.h"
#include "item.h"
#include "global_cells.h"
#include "fixed_math.h"
#include "layout_guard.h"
#include "a9_motion.h"
#include "ui.h"
#include "text/msg_ids.h"
struct ItemDefinition *Item_GetDirectFar(s32 item);


s32 Menu_ReservedStatusOne(void)
{
    return 1;
}

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu = gMenuWork;
    u16 *items;

    UnitGetFar(owner_id);
    ItemMenu_PosCategory();
    ItemMenu_HideAllIcons();
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipSlotLabels, window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipSlotLabels + 1, window, 0, 32);
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipSlotLabels + 2, window, 0, 16);
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipSlotLabels + 3, window, 0, 48);
    items = menu->items;
    ItemMenu_DrawEquippedItemNames(window, items);
    if (mode == 0) {
        WaitFrames(1);
        ItemMenu_DrawIcons(items, 1);
        ItemMenu_ArrangeCategoryItemIcons(items);
    }
}

#if EDITION_INTERNATIONAL
#define NAME_X 8
#else
#define NAME_X 16
#endif

void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items)
{
    s32 i;
    s32 item;

    for (i = 0; i < 15; i++) {
        if (items[i] & INVENTORY_EQUIPPED) {
            item = items[i] & ITEM_ID_MASK;
            switch (Item_GetDirectFar(item)->type) {
            case 1:
                UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, NAME_X, 8);
                break;
            case 2:
                UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, NAME_X, 56);
                break;
            case 3:
                UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, NAME_X, 40);
                break;
            case 4:
                UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, NAME_X, 24);
                break;
            }
        }
    }
}

void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 i;
    struct RenderOutput *obj;
    struct RenderOutput **tbl;

    i = 0;
    tbl =
        gMenuWork->entry_icons;
    do {
        obj = *tbl++;
        if (obj != NULL) {
            Menu_PlaceEntryObjectInGrid(obj, i, origin_x, origin_y, phase);
        }
        i += 1;
    } while (i <= 0x1F);
}

void Menu_PlaceEntryObjectInGrid(struct RenderOutput *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y =
        (no / phase * 0x10) + origin_y;
    obj->x =
        (no % phase * 0x10) + origin_x;
    UiIcon_PrepareObject(obj);
}

void ItemMenu_ArrangeCategoryItemIcons(u16 *items)
{
    struct InventoryMenuState *state;
    struct RenderOutput *icon;
    s32 i;

    state = gMenuWork;
    ItemMenu_PosCategory();
    for (i = 0; i < 15; i++) {
        if (items[i] != 0 && (items[i] & INVENTORY_EQUIPPED) != 0) {
            icon = state->entry_icons[i];
            if (icon != 0) {
                switch (Item_GetDirectFar(items[i] & ITEM_ID_MASK)->type) {
                case 1:
                    icon->x = 216;
                    icon->y = 32;
                    break;
                case 2:
                    icon->x = 216;
                    icon->y = 80;
                    break;
                case 3:
                    icon->x = 216;
                    icon->y = 64;
                    break;
                case 4:
                    icon->x = 216;
                    icon->y = 48;
                    break;
                }
                UiIcon_PrepareObject(icon);
            }
        }
    }
}
