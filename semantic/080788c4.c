#include "owner_state.h"
#include "types.h"

void Func_08077428(s32);

/*
 * Consume one unit from an inventory slot.  The upper five bits encode a
 * stacked quantity: decrementing them leaves the entry in place.  Once that
 * quantity reaches zero, remove the entry and compact all fifteen slots.
 *
 * Returns -1 for an empty slot, 1 for a decremented stack, and 2 when the
 * final unit removed the slot.  The owner's derived inventory state is
 * refreshed on every path.
 */
s32 Func_080788c4(s32 owner, s32 slot)
{
    struct OwnerInventoryState *state =
        (struct OwnerInventoryState *)Func_08077394(owner);
    u16 entry = state->inventory[slot];
    s32 result = -1;

    if (entry != 0) {
        if ((entry & 0xf800) != 0) {
            state->inventory[slot] = (u16)(entry - 0x0800);
            result = 1;
        } else {
            s32 read_index;
            s32 write_index = 0;

            state->inventory[slot] = 0;
            for (read_index = 0; read_index < 15; read_index++) {
                entry = state->inventory[read_index];
                if (entry != 0)
                    state->inventory[write_index++] = entry;
            }
            while (write_index < 15)
                state->inventory[write_index++] = 0;
            result = 2;
        }
    }

    Func_08077428(owner);
    return result;
}
