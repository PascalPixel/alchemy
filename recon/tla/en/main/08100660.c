/* Item menu: place the icon of each equipped item in the category page by
   item type: types 1 to 4 each have a row; other types keep their
   position. Each case stores its own column, as the reference keeps 216
   in a register across the loop. */
#include "ITEM.H"
#include "LAYOUT_GUARD.H"
#include "TYPES.H"

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

void ItemMenu_PosCategory(void);
void UiIcon_PrepareObject(void *icon);

void ItemMenu_ArrangeCategoryItemIcons(u16 *items)
{
    struct CategoryItemIconState *state;
    struct CategoryItemIcon *icon;
    s32 i;

    state = Data_03001f2c;
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
