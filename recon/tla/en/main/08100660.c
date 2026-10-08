#include "types.h"
#include "ram_buffer.h"
#include "item.h"

void ItemMenu_PosCategory(void);
void UiIcon_PrepareObject(void *arg0);

struct CategoryItemIcon {
    u8 reserved_00[6];
    s16 x;
    s16 y;
};

struct CategoryItemIconState {
    u8 reserved_00[76];
    struct CategoryItemIcon *icons[15];
};

void ItemMenu_ArrangeCategoryItemIcons(u16 *items)
{
    struct CategoryItemIconState *state;
    struct CategoryItemIcon *icon;
    s32 i;

    state = Ram_HeapSlots->menu_runtime;
    ItemMenu_PosCategory();
    for (i = 0; i < 15; i++) {
        if (items[i] != 0 && (items[i] & 0x200) != 0) {
            icon = state->icons[i];
            if (icon != 0) {
                switch (Item_Get(items[i] & 0x1ff)->type) {
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
