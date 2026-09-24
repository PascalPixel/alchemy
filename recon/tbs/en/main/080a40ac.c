/* Draft, not exact (2026-09-24): candidate=98 reference=100 differing_halfwords=26.
   The u16 mask is a plain 0x200, a halfword pool constant, which puts the
   literal pool mid-function as the reference does. Open: the reference
   enters the rotated slot loop by a jump to its bottom test (after loading
   slot 0 through the owner base), keeps the use count in r2 and copies it
   to the loop counter after its zero test, and sets the zero return before
   comparing the removal result with 2; a goto into a do-while loses the
   strength reduction. */
#include "TYPES.H"
#include "ITEM_MENU.H"
#include "OWNER_STATE.H"

s32 Func_08077058(s32 owner, s32 slot);

s32 Func_080a40ac(s32 owner_id)
{
    struct OwnerInventoryState *owner;
    s32 slot;
    s32 extra;
    s32 quantity;
    s32 result;

    owner = OwnerState_GetFar(owner_id);
    result = 0;
    slot = 0;

    while (owner->inventory[slot] != 0) {
        if ((owner->inventory[slot] & 0x200) == 0) {
            extra = owner->inventory[slot] >> 11;
            quantity = extra + 1;
            if (extra == 0)
                quantity = 1;

            while (quantity != 0) {
                result = Func_08077058(owner_id, slot);
                quantity--;
            }

            if (result != 2)
                return 0;
            break;
        }

        slot++;
        if (slot > 14)
            return result;
    }

    return 1;
}
