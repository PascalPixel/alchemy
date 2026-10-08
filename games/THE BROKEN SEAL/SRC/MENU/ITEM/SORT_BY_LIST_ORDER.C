#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "ITEM.H"
struct ItemDefinition *Item_GetDirectFar(s32 item);


s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode)
{
    u16 sorted[16];
    u16 items[16];
    u8 order[32];
    struct ItemDefinition *item;
    s32 pass;
    s32 i;
    s32 count;
    s32 best;
    s32 best_slot;
    s32 sorted_count;
    s32 value;

    best_slot = 0;
    sorted_count = 0;
    Menu_CopyListOrder(mode, order);

    for (i = 0; i < 15; i++)
        items[i] = inventory[i];

    count = 0;
    for (i = 0; i < 15; i++) {
        if (items[i] != 0)
            count++;
    }

    for (i = count; i < 15; i++)
        sorted[i] = 0;

    pass = 0;
    while (order[pass] != 0xff) {
        for (;;) {
            best = 0;
            for (i = 0; i < count; i++) {
                if (items[i] == 0)
                    continue;
                item = Item_GetDirectFar(items[i]);
                if ((order[pass] & 0x7f) != item->type)
                    continue;
                if ((order[pass] & 0x80) && (items[i] & 0x200) == 0)
                    continue;
                value = items[i] & 0x1ff;
                if (best < value) {
                    best_slot = i;
                    best = value;
                }
            }
            if (best == 0)
                break;
            sorted[sorted_count] = items[best_slot];
            items[best_slot] = 0;
            sorted_count++;
        }
        pass++;
    }

    for (i = 0; i < count; i++)
        inventory[i] = sorted[i];
    return 1;
}
