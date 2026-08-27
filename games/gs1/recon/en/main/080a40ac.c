#include "inventory_menu.h"
#include "owner_state.h"

s32 Func_08077058(s32 owner, s32 slot);

s32 Func_080a40ac(s32 owner_id)
{
    struct OwnerInventoryState *owner;
    u16 *slots;
    s32 slot;
    s32 encoded;
    s32 quantity;
    s32 flag;
    s32 result;

    owner = OwnerState_GetFar(owner_id);
    slots = owner->inventory;
    flag = 0;
    slot = 0;

    while ((encoded = slots[slot]) != 0) {
        if ((encoded & 0x200) == 0) {
            quantity = (u32)encoded >> 11;
            quantity = (quantity != 0) ? quantity + 1 : 1;

            if (quantity != 0) {
                do {
                    flag = Func_08077058(owner_id, slot);
                    quantity--;
                } while (quantity != 0);
            }

            if (flag == 2)
                goto found;
            goto done;
        }

        slot++;
        if (slot > 14)
            goto done;
    }

found:
    flag = 1;
done:
    result = flag;
    return result;
}
