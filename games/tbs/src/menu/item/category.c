#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "character_menu.h"
#include "types.h"
#include "inventory_menu.h"
#include "layout_guard.h"
#include "object_factory.h"
#include "global_cells.h"



void ItemMenu_PosCategory(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 x = 248;
    struct RenderOutput **entry = state->entry_icons;
    s32 y = 168;
    s32 remaining = 31;

    do {
        struct RenderOutput *object = *entry++;

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
    struct RenderOutput **slot;
    struct RenderOutput *obj;
    s32 i;
    struct InventoryMenuState *state;
    s32 param;

    state = gMenuWork;
    i = 0;
    param = 0xA8;
    slot = state->category_icons;
    do {
        obj = RenderOutput_CreateFromResourceFar(2, i, (struct RenderInput *)resource, 0xF8, param);
        i += 1;
        *slot = obj;
        slot += 1;
    } while (i <= 7);
    return 1;
}

void ItemMenu_ApplyFlags(const u8 *flags)
{
    /* FAKEMATCH: the y-field pointer keeps the category flag index as a counter instead of a walking pointer. */
    struct InventoryMenuState *base;
    struct RenderOutput **slot;
    struct RenderOutput *entry;
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
        struct RenderOutput *object = state->category_icons[index];

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
            case CHARACTER_DOWN: kind = 16; break;
            case CHARACTER_POISON: kind = 1; break;
            case CHARACTER_VENOM: kind = 2; break;
            case CHARACTER_CURSE: kind = 15; break;
            case CHARACTER_HAUNT: kind = 7; break;
            default: kind = 0; break;
            }
            Resource_LoadByModeIntoSlotFar(8, kind, (u8)state->category_icons[index]->index, 0);
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
