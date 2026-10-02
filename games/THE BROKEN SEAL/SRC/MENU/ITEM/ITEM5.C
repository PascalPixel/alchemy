#include "TYPES.H"
#include "INVENTORY_MENU.H"
#include "LAYOUT_GUARD.H"
#include "OBJECT_FACTORY.H"
#include "GLOBAL_CELLS.H"

void UiIcon_PrepareObject(void *obj);
void ItemMenu_ResetCategory(void);

void Palette_CopyObjectBankToBackground14(void);
void Palette_LightenBankHighlight(s32);

void ItemMenu_PosCategory(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 x = 248;
    struct InventoryMenuIcon **entry = state->entry_icons;
    s32 y = 168;
    s32 remaining = 31;

    do {
        struct InventoryMenuIcon *object = *entry++;

        if (object != 0) {
            object->x = x;
            object->y = y;
            UiIcon_PrepareObject(object);
        }
        remaining--;
    } while (remaining >= 0);
}

s32 Menu_CreateEightEntryObjects(s32 resource)
{
    struct InventoryMenuIcon **slot;
    struct InventoryMenuIcon *obj;
    s32 i;
    struct InventoryMenuState *state;
    s32 param;

    state = gMenuWork;
    i = 0;
    param = 0xA8;
    slot = state->category_icons;
    do {
        obj = (struct InventoryMenuIcon *)RenderOutput_CreateFromResourceFar(2, i, resource, 0xF8, param);
        i += 1;
        *slot = obj;
        slot += 1;
    } while (i <= 7);
    return 1;
}

/* Item menu: after resetting the category, place the flagged entries of
   the five category sprites in a column 16 pixels apart, starting at 88.

   FAKEMATCH: the y store goes through its own pointer, which is what keeps
   the flag index as a counter over the flags base (loop.c otherwise turns
   flags[index] into a walking pointer). */
void ItemMenu_ApplyFlags(const u8 *flags)
{
    struct InventoryMenuState *base;
    struct InventoryMenuIcon **slot;
    struct InventoryMenuIcon *entry;
    s32 index;
    s32 value;
    u16 kind;

    base = gMenuWork;
    ItemMenu_ResetCategory();
    index = 0;
    slot = base->category_icons;
    value = 88;
    do {
        entry = *slot++;
        if (entry != 0 && flags[index] != 0) {
            kind = 8;
            entry->x = kind;
            {
                s16 *y = &entry->y;

                *y = value;
            }
            entry->sentinel = 240;
            UiIcon_PrepareObject(entry);
            value += 16;
        }
        index++;
    } while (index <= 4);
}

void ItemMenu_ResetCategory(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 index;

    for (index = 0; index < 5; index++) {
        struct InventoryMenuIcon *object = state->category_icons[index];

        if (object != 0) {
            object->x = 248;
            object->y = 168;
            object->sentinel = 240;
            UiIcon_PrepareObject(object);
        }
    }
}

s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 index = 0;

    do {
        if (enabled[index] != 0) {
            s32 kind;
            switch (index) {
            case 0: kind = 16; break;
            case 1: kind = 1; break;
            case 2: kind = 2; break;
            case 3: kind = 15; break;
            case 4: kind = 7; break;
            default: kind = 0; break;
            }
            Resource_LoadByModeIntoSlotFar(8, kind, state->category_icons[index]->render_target, 0);
        }
        index++;
    } while (index <= 4);
    return 1;
}

void Item_PrepareUsePalette(void)
{
    Palette_CopyObjectBankToBackground14();
    Palette_LightenBankHighlight(13);
}

void Item_UseNoOpCallback(void)
{
}
