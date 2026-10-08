#include "types.h"
#include "owner_state.h"

s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item)
{
    struct BtlUnit *state;
    s32 i;
    s32 quantity;

    quantity = 0;
    state = UnitGetFar(owner);
    i = 0;
    do {
        if (state->inventory[i] != 0 && (state->inventory[i] & 0x1ff) == item) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            quantity += (state->inventory[i] & 0xf800) >> 11;
            quantity++;
#else
            quantity = (state->inventory[i] & 0xf800) >> 11;
            quantity++;
            break;
#endif
        }
        i++;
    } while (i <= 14);
    return quantity;
}
