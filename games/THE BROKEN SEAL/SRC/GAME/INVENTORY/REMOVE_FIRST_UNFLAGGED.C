/* Inventory: remove the first stack without the 0x200 flag from an owner's
   bag, one removal per unit it holds.  Returns 1 when the last removal
   reports 2 or when the bag ends before such a stack, 0 otherwise. */
#include "INVENTORY.H"

s32 Inventory_RemoveFar(s32 owner, s32 slot);

s32 Inventory_RemoveFirstUnflagged(s32 owner)
{
    struct OwnerInventoryState *state = OwnerState_GetFar(owner);
    s32 result = 0;
    s32 i;

    for (i = 0; i < 15; i++) {
        if (state->inventory[i] == 0) {
            result = 1;
            break;
        }
        if ((state->inventory[i] & 0x200) == 0) {
            s32 quantity = state->inventory[i] >> 11;
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
