/*
 * Draft: ItemMenu_ArrangeCategoryItemIcons, ported from its ☀️ twin with the
 * menu work from its heap slot and ⚓️'s icons at 0x4c. Remaining difference:
 * two scheduling swaps in the loop (the listing advances the item pointer
 * before copying the item, and spills r1 before masking the id).
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "ITEM.H"

void ItemMenu_PosCategory(void);
void UiIcon_PrepareObject(void *arg0);

struct CategoryItemIcon {
    u8 reserved_00[6];
    s16 x;
    s16 y;
};

/* ⚓️ keeps the item icons four bytes later than ☀️. */
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
