#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "inventory.h"


s32 Inventory_RemoveFirstUnflagged(s32 owner)
{
    struct OwnerInventoryState *state = UnitGet(owner);
    s32 result = 0;
    s32 i;

    for (i = 0; i < INVENTORY_SLOTS; i++) {
        if (state->inventory[i] == 0) {
            result = 1;
            break;
        }
        if ((state->inventory[i] & INVENTORY_EQUIPPED) == 0) {
            s32 quantity = state->inventory[i] >> INVENTORY_QUANTITY_SHIFT;
            s32 count = quantity + 1;
            s32 cnt;

            if (quantity == 0)
                count = 1;
            for (cnt = 0; cnt < count; cnt++)
                result = Inventory_RemoveFar(owner, i);
            if (result != 2)
                return 0;
            result = 1;
            break;
        }
    }
    return result;
}
