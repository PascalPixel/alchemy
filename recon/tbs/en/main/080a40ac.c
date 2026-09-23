/* Draft, not exact (2026-09-24): candidate=100 reference=100 differing_halfwords=34. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_00000200;
#include "ITEM_MENU.H"
#include "OWNER_STATE.H"

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
        if ((encoded & (s32)&Value_00000200) == 0) {
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
            return 0;
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
