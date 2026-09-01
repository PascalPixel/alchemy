#include "item.h"
#include "layout_guard.h"
#include "types.h"

struct CategoryItemIcon {
    u8 reserved_00[6];
    s16 x;
    s16 y;
};

struct CategoryItemIconState {
    u8 reserved_00[72];
    struct CategoryItemIcon *icons[15];
};

LAYOUT_OFFSET_GUARD(CategoryItemIcon_X, struct CategoryItemIcon, x, 6);
LAYOUT_OFFSET_GUARD(CategoryItemIcon_Y, struct CategoryItemIcon, y, 8);
LAYOUT_OFFSET_GUARD(
    CategoryItemIconState_Icons,
    struct CategoryItemIconState,
    icons,
    72);

extern struct CategoryItemIconState *Data_03001f2c;

void InventoryMenu_PositionCategoryItems(void);
void UiIcon_PrepareObject(void *icon);

#define InventoryMenu_ArrangeCategoryItemIcons Func_080a9c18

void InventoryMenu_ArrangeCategoryItemIcons(const u16 *items)
{
    struct CategoryItemIconState *state = Data_03001f2c;
    struct CategoryItemIcon **icon_entry;
    const u16 *item_entry;
    s32 remaining;

    InventoryMenu_PositionCategoryItems();
    remaining = 14;
    icon_entry = state->icons;
    item_entry = items;
    do {
        u16 encoded_item = *item_entry++;

        if (encoded_item != 0 && (encoded_item & 0x200) != 0) {
            struct CategoryItemIcon *icon = *icon_entry;

            if (icon != 0) {
                struct ItemDefinition *item;
                s32 y;

                item = Item_Get(encoded_item & 0x1ff);
                switch (item->type) {
                case 1:
                    y = 32;
                    break;
                case 2:
                    y = 80;
                    break;
                case 3:
                    y = 64;
                    break;
                case 4:
                    y = 48;
                    break;
                default:
                    UiIcon_PrepareObject(icon);
                    goto next;
                }
                icon->x = 216;
                icon->y = y;
                UiIcon_PrepareObject(icon);
            }
        }
next:
        remaining--;
        icon_entry++;
    } while (remaining >= 0);
}
