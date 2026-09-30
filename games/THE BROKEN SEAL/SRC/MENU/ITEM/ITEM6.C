#include "TYPES.H"
#include "INVENTORY_MENU.H"
#include "OWNER_STATE.H"
#include "SYSTEM.H"
#include "ITEM.H"
#include "A9_MOTION.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"
#include "LAYOUT_GUARD.H"

extern u8 MsgEquipSlotLabels;
extern void ItemMenu_PosCategory(void);
extern void UiText_DrawCharacterAtOffsetFar(void *, s32, s32, s32);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
extern u8 MsgItemName;
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items);
extern u8 Data_03001f2c[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
void UiIcon_PrepareObject(void *arg0);

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
void ItemMenu_PosCategory(void);
void UiIcon_PrepareObject(void *icon);

s32 Menu_ReservedStatusOne(void)
{
    return 1;
}

/* Item menu: write the names of the equipped items into the equipment
   window, one row per item type 1 to 4. The name message is the item id
   plus 0x182; its pool constant is hoisted into a register for the loop. */
void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu = gMenuWork;
    u8 *items;

    Owner_GetStateFar(owner_id);
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

/* Where the equipped names start: the Japanese edition indents them. */
#if defined(TBS_EDITION_JA)
#define NAME_X 16
#else
#define NAME_X 8
#endif

void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items)
{
    s32 i;
    s32 item;

    for (i = 0; i < 15; i++) {
        if (items[i] & 0x200) {
            item = items[i] & 0x1ff;
            switch (Item_Get(item)->type) {
            case 1:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, NAME_X, 8);
                break;
            case 2:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, NAME_X, 56);
                break;
            case 3:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, NAME_X, 40);
                break;
            case 4:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, NAME_X, 24);
                break;
            }
        }
    }
}

void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 i;
    struct Object080a9bd8 *obj;
    struct Object080a9bd8 **tbl;

    i = 0;
    tbl =
        (struct Object080a9bd8 **)(*(s32 *)((u32)&Data_03001f2c) + 0x48);
    do {
        obj = *tbl++;
        if (obj != NULL) {
            Menu_PlaceEntryObjectInGrid(obj, i, origin_x, origin_y, phase);
        }
        i += 1;
    } while (i <= 0x1F);
}

void Menu_PlaceEntryObjectInGrid(struct Object080a9bd8 *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y =
        (s16)((Math_Div(no, phase) * 0x10) + origin_y);
    obj->x =
        (s16)((Math_Mod(no, phase) * 0x10) + origin_x);
    UiIcon_PrepareObject(obj);
}

/* Item menu: place the icon of each equipped item in the category page by
   item type: types 1 to 4 each have a row; other types keep their
   position. Each case stores its own column, as the reference keeps 216
   in a register across the loop. */
void ItemMenu_ArrangeCategoryItemIcons(u16 *items)
{
    struct CategoryItemIconState *state;
    struct CategoryItemIcon *icon;
    s32 i;

    state = *(struct CategoryItemIconState **)Data_03001f2c;
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
